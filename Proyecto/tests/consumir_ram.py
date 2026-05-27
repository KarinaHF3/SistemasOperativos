import time

a = []

while True:
    bloque = bytearray(150_000_000)  # 150 MB

    for i in range(0, len(bloque), 4096):
        bloque[i] = 1

    a.append(bloque)

    print(f"Memoria reservada: {len(a) * 150} MB")

    time.sleep(0.5)