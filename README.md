# Technical Documentation — Balatro

## Overview

Balatro adalah sistem evaluasi tangan poker dan perhitungan skor berbasiskan C++ yang
diinspirasi oleh permainan roguelike berbasis poker. Sistem ini dirancang untuk:

- Mengevaluasi tangan poker
- Menghitung skor dasar
- Menerapkan efek joker
- Mengelola aksi pemain (PLAY / DISCARD)

Arsitektur bersifat modular: setiap subsistem memiliki tanggung jawab jelas dan dapat
diperluas tanpa merombak keseluruhan kode.

## High-Level Architecture

| System             | Responsibility                            |
|--------------------|-------------------------------------------|
| GameManager        | Controls the overall game flow            |
| Deck               | Stores and manages cards                  |
| HandState          | Stores cards currently held by the player |
| ChosenHand         | Stores cards selected for scoring         |
| PokerHandEvaluator | Detects poker hand types                  |
| ScoringRule        | Calculates base scores                    |
| JokerManager       | Applies joker effects                     |
| ScoreContext       | Stores mutable score data                 |

## Runtime Flow

main() → GameManager::runSession() → Setup Jokers → Create Deck → Shuffle Deck →
Draw Initial Hand → Player Selects Cards → Evaluate Hand → Calculate Base Score →
Apply Joker Effects → Print Final Result

## Card System

### Card
Representasi satu kartu bermain (rank: Two..Ace, suit: Hearts/Diamonds/Clubs/Spades).

### Deck
Tanggung jawab: membuat 52 kartu, mengacak (std::shuffle + std::mt19937), draw, dan memantau sisa kartu.

Fungsi utama:
- `initialize()` — buat 52 kartu
- `shuffle()` — randomize
- `drawCard()` — ambil kartu teratas
- `size()` / `isEmpty()`

## Hand System

- `HandState` menyimpan kartu pemain saat ini.
- `ChosenHand` menyimpan kartu yang dipilih untuk scoring.
- `DrawService` dan `DiscardService` mengelola pengambilan dan pembuangan kartu.

## Poker Hand Evaluation

`PokerHandEvaluator` menyimpan daftar `PokerHandChecker` dan menjalankan pengecekan
berurut dari yang terkuat ke yang terlemah. Checker pertama yang match menjadi hasil akhir.

Prioritas (kuat → lemah): FlushFive, FlushHouse, FiveOfAKind, RoyalFlush, StraightFlush,
FourOfAKind, FullHouse, Flush, Straight, ThreeOfAKind, TwoPair, Pair, HighCard.

Setiap checker mengimplementasikan:
- `checkPokerHand(const std::vector<Card>& cards)`
- `getHandType()`

## Scoring System

`HandResolver` menggabungkan evaluasi tangan dan aturan scoring, menghasilkan `PlayedHandResult`:

- `cards`, `handType`, `level`, `chips` (base chips), `mult` (base multiplier), `finalScore`.

Formula dasar:

Final Score = chips × mult

Contoh (dokumentasi): Pair = 10 chips × 2 mult, Flush = 35 chips × 4 mult.

## Joker System

`ScoreContext` adalah objek runtime yang mutable — jokers memodifikasi `chips`, `mult`, dan/atau konteks.
`JokerManager` menyimpan dan memberitahukan semua joker saat skor dasar dihitung.

Contoh joker bawaan:
- `PairJoker` — aktif saat hand type `Pair`, efek: +4 mult
- `FlatChipJoker` — selalu aktif, efek: +20 chips

## Extension Guide

Menambah Joker:
1. Buat kelas turunan `Joker` (contoh `MyNewJoker : public Joker`).
2. Implementasikan: `getName()`, `getDescription(const ScoreContext&)`, `onScoreCalculated(ScoreContext&)`.
3. Daftarkan di `GameManager::setupJokers()` melalui `jokerManager.addJoker(...)`.

Menambah Poker Hand:
1. Tambah nilai di `PokerHandType`.
2. Buat `PokerHandChecker` baru implementasi `checkPokerHand` + `getHandType`.
3. Registrasikan checker di `PokerHandEvaluator` sesuai urutan prioritas.
4. Tambah data skor di `HandScoreTable`.

## Blind State & Skip Reward (Arsitektur Baru)

Untuk memisahkan logika blind progression dan skip reward, proposal menggunakan dua pattern:

1. State Pattern — tiap blind adalah objek `BlindState` yang bertanggung jawab menentukan
	 nama, target score, reward money, perintah skip reward, dan state berikutnya.

	 Interface contoh:

	 ```cpp
	 class BlindState {
	 public:
		 virtual ~BlindState() = default;
		 virtual std::string getName() const = 0;
		 virtual int getTargetScore(int ante) const = 0;
		 virtual int getRewardMoney() const = 0;
		 virtual PendingCommand createSkipRewardCommand() const = 0;
		 virtual std::unique_ptr<BlindState> nextState(int& ante) const = 0;
	 };
	 ```

	 Contoh: `SmallBlindState`, `BigBlindState`, `BossBlindState` (boss menaikkan `ante`).

2. Command Pattern — skip reward direpresentasikan sebagai `RewardCommand` yang dapat dieksekusi
	 nanti. Command dibungkus dalam `PendingCommand` berisi `CommandTiming` (Immediate, NextBlind, NextShop, NextAnte)
	 dan disimpan di `RunSessionState` sampai waktunya dieksekusi.

	 Contoh `RewardCommand`:

	 ```cpp
	 class RewardCommand {
	 public:
		 virtual ~RewardCommand() = default;
		 virtual std::string getName() const = 0;
		 virtual std::string getDescription() const = 0;
		 virtual void execute(RunSessionState& state) = 0;
	 };
	 ```

## RunSession Integration (Ringkas)

- Saat pemain `skipBlind()`: `createSkipRewardCommand()` dipanggil pada `currentBlind`,
	`PendingCommand` disimpan ke `state.pendingCommands`, lalu `currentBlind` diganti ke `nextState()`.
- Sistem memanggil `executePendingCommands(state, timing)` pada event terkait (NextBlind, NextShop, NextAnte)
	untuk mengeksekusi perintah yang waktunya sesuai.

Contoh perubahan `RunSessionState`:

```cpp
struct RunSessionState {
	int ante = 1;
	int totalScore = 0;
	int remainingPlays = 4;
	int remainingDiscards = 3;
	int freeRerolls = 0;
	std::unique_ptr<BlindState> currentBlind;
	std::vector<PendingCommand> pendingCommands;
};
```

## Keuntungan Arsitektur

- Blind progression modular dan terisolasi per kelas.
- Skip reward scalable: cukup menambah `RewardCommand` baru.
- Deferred reward didukung secara natural lewat `PendingCommand` + `CommandTiming`.
- Maintainability dan extensibility meningkat.

## Kesimpulan

Proposal ini memisahkan concern:

- Blind progression → State Pattern
- Reward behavior → Command Pattern
- Deferred reward → Deferred Command Queue

---

Jika kamu mau, saya bisa:

1. Menambahkan header dan implementasi contoh (`include/BlindState.h`, `include/RewardCommand.h`, `include/PendingCommand.h`) ke repo.
2. Menulis contoh unit test kecil untuk alur skip reward.

Pilih nomor jika ingin saya lanjutkan dengan kode.
