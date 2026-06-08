# Game Flow Diagram — Balatro

## Ringkasan Alur Eksekusi

```mermaid
flowchart TD
    A[main()]
    B[GameManager::runSession()]
    C[Set currentBlind jika belum ada]
    D[setupJokers()]
    E[initializeStandardDeck() + shuffleDeck()]
    F[Mulai Loop Blind (3 kali)]
    G[executePendingCommands(currentBlind timing)]
    H[promptPlayerAction()]
    I{Pemain pilih}
    J[skipBlind()]
    K[Membuat PendingCommand + nextState()]
    L[drawCards(8)]
    M[Hitung target score]
    N[Gameplay loop]
    O{Pilih action}
    P[Discard kartu]
    Q[Play kartu]
    R[Bangun playedHand]
    S[ScoreContext + scoringRule->scoreHand()]
    T[applyAllEffects(context)]
    U[Update currentScore]
    V{Score >= target?}
    W[Blind cleared -> reward -> nextState]
    X[Game Over]
    Y[executePendingCommands(CommandTiming::Start)]
    Z[End run]

    A --> B
    B --> C
    C --> D
    D --> E
    E --> F
    F --> G
    G --> H
    H --> I
    I -- "SKIP" --> J
    I -- "PLAY" --> L
    J --> K
    K --> G
    K --> F
    L --> M
    M --> N
    N --> O
    O -- "DISCARD" --> P
    O -- "PLAY" --> Q
    P --> N
    Q --> R
    R --> S
    S --> T
    T --> U
    U --> V
    V -- "YA" --> W
    V -- "TIDAK" --> N
    N --> X
    F --> Y
    Y --> Z
```

> Jika diagram Mermaid tidak tampil di preview MDX, gunakan preview Markdown bawaan VS Code atau instal ekstensi yang mendukung Mermaid seperti `Markdown Preview Enhanced`.

## Fallback Diagram Teks

main() -> GameManager::runSession() ->
  - Inisialisasi currentBlind bila belum ada
  - setupJokers()
  - initializeStandardDeck() + shuffleDeck()
  - Loop Blind (3x):
    - executePendingCommands(currentBlind timing)
    - promptPlayerAction()
    - Jika SKIP:
      - skipBlind()
      - buat PendingCommand
      - currentBlind = nextState()
    - Jika PLAY:
      - drawCards(8)
      - hitung target score
      - Gameplay loop sampai remainingPlays habis:
        - pilih DISCARD atau PLAY
        - DISCARD: buang kartu
        - PLAY: bangun playedHand, scoreHand(), applyAllEffects(), update currentScore
        - jika score >= target: blind cleared, reward, nextState()
        - jika tidak: refill kartu dan lanjut
      - jika selesai tanpa target: Game Over
  - setelah loop blind: executePendingCommands(CommandTiming::Start)
  - End run

## Penjelasan Elemen Utama

1. `main()`
   - Membuat objek `GameManager`
   - Memanggil `runSession()`

2. `GameManager::runSession()`
   - Inisialisasi `sessionState.currentBlind` bila kosong
   - Panggil `setupJokers()` untuk mendaftarkan joker aktif
   - Inisialisasi deck, shuffle, dan siapkan loop blind

3. Loop Blind
   - Set ulang `remainingPlays`, `remainingDiscards`, `currentScore`
   - Eksekusi pending commands untuk timing blind sekarang
   - Tanyakan pemain: `PLAY` atau `SKIP`

4. Skip Blind
   - `skipBlind()` membuat `PendingCommand` skip reward
   - Pindah ke blind berikutnya
   - Eksekusi pending commands baru jika sesuai timing

5. Play Hand
   - Ambil 8 kartu awal
   - Hitung target blind
   - Masuk loop gameplay sampai giliran habis
   - Pilih `DISCARD` untuk membuang kartu atau `PLAY` untuk menghitung skornya

6. Scoring dan Joker
   - Buat `ScoreContext`
   - Hitung skor awal dengan `ScoringRule`
   - Terapkan efek joker via `jokerManager`
   - Tambahkan hasil ke `currentScore`
   - Refill kartu sampai 8 bila perlu

7. Resolusi Blind
   - Jika target tercapai: sukses, berikan reward, pindah blind
   - Bila tidak tercapai sampai habis giliran: game over dan hentikan run

8. Akhir Session
   - Jalankan `executePendingCommands(sessionState, CommandTiming::Start)`
   - Cetak akhir run

## Detail Flow Tambahan

- `setupJokers()` mendaftarkan joker seperti `FlatChipJoker` dan `PairJoker`
- `RunSessionService` bertanggung jawab untuk `skipBlind()` dan menjalankan `PendingCommand`
- `PendingCommand` dieksekusi berdasarkan `CommandTiming` (misalnya: `Start`, `NextBlind`, `NextAnte`)
- `BlindState` menentukan target, reward, dan state berikutnya ketika blind berhasil atau dilewati
