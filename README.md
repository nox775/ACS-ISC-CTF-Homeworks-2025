# ACS ISC CTF Homeworks 2025

Acest repository conține soluțiile mele (write-ups & exploits) pentru Tema 1 la materia **Introduction to Computer Security (ISC) 2025**.

## Sumar Teme

| Task | Tip | Status | Descriere Scurtă |
| :--- | :--- | :--- | :--- |
| **Temu RSA** | Crypto | ✅ Solved | RSA (dp=p) + Vigenère Cipher |
| **Minishell** | Linux / Shell | ✅ Solved | Path Traversal + Command Injection |
| **CSV Grader** | Binary Exploit | ✅ Solved | Buffer Overflow + ROP Chain |
| **Micropubsub**| Binary Exploit | ⚠️ Partial | Heap Overflow + Stack Pivot |

---

## 🛠️ Detalii Tehnice & Soluții

### 1. Temu RSA (Crypto)
**Fișier:** `task1-temu-rsa/decrypt.py`

Analizând parametrii RSA oferiți, am observat că valoarea etichetată `dp` era de fapt chiar numărul prim `p`.
* **Vulnerabilitate:** Expunerea parametrului `p` permite calcularea imediată a lui `q` și a cheii private `d`.
* **Layer 2:** După decriptarea RSA, textul rezultat era încă ininteligibil. A fost necesară o decriptare **Vigenère** folosind cheia `"CROISSANT"` pentru a obține steagul final (format `SpeishFlag{...}BAGUETTE`).

### 2. Minishell (Linux & ACLs)
**Log:** `task2-minishell/terminal_log.txt`

Un shell restricționat care permitea citirea/scrierea fișierelor, dar filtra anumite path-uri.
* **Vulnerabilitate:** Lipsa sanitizării input-ului a permis **Path Traversal** (`../../../`).
* **Exploit:**
    1. Am localizat scriptul `/home/accountant/run_report.sh` care era executat de binarul SUID `/opt/minishell/audit`.
    2. Am injectat un payload în acest script folosind comanda `write`.
    3. Payload-ul a folosit sintaxa `$(...)` în argumentele către `audit` pentru a executa comenzi ca root (deoarece `audit` rula cu privilegii ridicate).
    4. **Rezultat:** Citirea fișierului protejat `/etc/X11/not_for_your_eyes/zflag.x`.

### 3. CSV Grader (Binary Exploitation)
**Exploit:** `task3-csvgrader/fuzz.py`

Un parser de CSV vulnerabil la Buffer Overflow din cauza utilizării incorecte a `sscanf` (se oprea la virgulă, permițând overflow în câmpul `username`).
* **Offset:** 62 bytes.
* **Tehnica:** **ROP (Return Oriented Programming)**.
* **Chain:**
    1. `pac_pac(0xb659308)` -> Încarcă flag-ul în variabila globală `fl00g`.
    2. `puts(fl00g)` -> Afișează conținutul variabilei la stdout.
    3. `exit(0)` -> Terminare curată.

### 4. Micropubsub (Binary Exploitation - Hard)
**Exploit:** `task4-pubsub/exploit.py`

*Note: Soluție parțială.*
* **Analiză:** Binarul prezintă un overflow în zona DATA care permite suprascrierea pointer-ului funcției `logfunc`.
* **Abordare:**
    * Am suprascris `logfunc` pentru a redirecționa execuția către un gadget de **Stack Pivot** (`add esp, 0x7c ; ret`).
    * Am construit un ROP chain care încearcă să apeleze `write(1, flag_addr, 60)`.
    * Deși mecanismul de control al fluxului de execuție (EIP control) funcționează, extragerea finală a flag-ului necesită ajustări suplimentare.

---

## ⚠️ Disclaimer
Aceste scripturi sunt strict în scop educațional, realizate pentru rezolvarea temei din cadrul cursului ISC 2025.
