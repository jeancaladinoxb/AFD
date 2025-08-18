def cargar_configuracion(archivo):
    estados, alfabeto, transiciones, inicial, finales = [], [], {}, None, []
    with open(archivo, 'r') as f:
        for linea in f:
            linea = linea.strip()
            if linea.startswith("#") or not linea:
                continue
            # Estados
            if linea.startswith("Estados="):
                estados = linea.split("=")[1].strip().split(",")
            # Alfabeto
            elif linea.startswith("Alfabeto="):
                alfabeto = linea.split("=")[1].strip().split(",")
            
            # Inicial
            elif linea.startswith("Inicial="):
                inicial = linea.split("=")[1].strip()
            
            # Finales
            elif linea.startswith("Finales="):
                finales = linea.split("=")[1].strip().split(",")
            
            # Transiciones
            else:
                partes = [p.strip() for p in linea.split(",")]
                if len(partes) == 3:
                    e1, simbolo, e2 = partes
                    transiciones[(e1, simbolo)] = e2
                else:
                    print(f"Línea inválida ignorada en Conf.txt: {linea}")

    return estados, alfabeto, transiciones, inicial, finales

def procesar_cadena(cadena, transiciones, inicial, finales):
    estado = inicial
    recorrido = [estado]  # guardamos el recorrido
    for simbolo in cadena.split():
        if (estado, simbolo) in transiciones:
            estado = transiciones[(estado, simbolo)]
            recorrido.append(estado)
        else:
            print(f"Transición no encontrada: ({estado}, {simbolo})")
            return False, recorrido
    return (estado in finales), recorrido


if __name__ == "__main__":
    estados, alfabeto, transiciones, inicial, finales = cargar_configuracion("Conf.txt")

    with open("Cadenas.txt", "r") as f:
        for linea in f:
            cadena = linea.strip()
            aceptada, recorrido = procesar_cadena(cadena, transiciones, inicial, finales)
            print(f"Cadena: {cadena}")
            print("Recorrido:", " -> ".join(recorrido))
            if aceptada:
                print("ACEPTADA\n")
            else:
                print("RECHAZADA\n")

