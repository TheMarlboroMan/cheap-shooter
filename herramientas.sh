#!/bin/bash

# Pequeño script para ayudar con la compilacion.

clear #Limpiar pantalla

while [ "$opcion" != "0" ]
do
	echo "[a]Make motor"
	echo "[b]Make main"
	echo "[c]Make completo"
	echo "[d]Lanzar GDB"
	echo "[e]Lanzar valgrind full"
	echo "[f]Lanzar valgrind full reachable"
	echo "[g]Compilar lector perfiles"
	echo "[h]Limpiar nivel 1"
	echo "[j]Limpiar compilacion motor"
	echo "[k]Limpiar compilacion aplicacion"
	echo "[l]Limpiar compilacion completa"
	echo "[p]Limpiar perfiles"
	echo "[r]Limpiar copias de seguridad"
	echo "[t]Localizar TODO"
	echo "[y]Limpiar puntuaciones"
	echo "[z]Limpiar"
	echo "[1]Ejecutar main (./a.out)"
	echo "[2]Ejecutar editor (./a.out editor data/niveles/nivel04.dat)"
	echo "[0]Salir"

	read opcion

	case "$opcion" in

		"z" | "Z" )
			clear
		;;

		"p" | "P" )
			rm data/perfiles/*
		;;

		"h" | "H" )
			rm data/niveles/nivel01.dat
		;;

		"l" | "L" )
			rm ./objects/*
			rm ./objects_motor_sdl/*
			rm ./libDanSDL.a
			rm ./a.out
			rm ./lector_perfiles
			echo "Listo"
		;;

		"k" | "K" )
			rm ./objects/*
			rm ./a.out
			echo "Listo"
		;;

		"j" | "J" )
			rm ./objects_motor_sdl/*
			rm ./libDanSDL.a
			echo "Listo"
		;;

		"g" | "G" )
			g++ -std=c++0x -o lector_perfiles lector_perfiles.cpp
			echo "Listo"
		;;

		"t" | "T" )
			grep -r '//TODO' *
			echo "Listo"
		;;

		"r" | "R" )
			find ./ -name '*~' -exec rm '{}' \; -print -or -name ".*~" -exec rm {} \; -print
			echo "Listo"
		;;

		"y" | "Y" )
			rm ./data/puntuaciones.dat
			echo "Listo"
		;;

		"a" | "A" )
			make -sf class/motor_sdl/makefile_motor_sdl;
			echo "Listo"
		;;

		"b" | "B" )
			make -sf makefile_main;
			echo "Listo"
		;;

		"c" | "C" )
			make -f class/motor_sdl/makefile_motor_sdl;
			make -f makefile_main;
			echo "Listo"
		;;
	
		"d" | "D" )
			gdb ./a.out
		;;

		"e" | "E" )
			valgrind --leak-check=full --log-file=resultados_valgrind.txt --track-origins=yes ./a.out
		;;

		"f" | "F" )
			valgrind --leak-check=full --log-file=resultados_valgrind.txt --show-reachable=yes ./a.out
		;;

		"1")
			./a.out
			echo "Listo"
		;;

		"2")
			./a.out editor data/niveles/nivel04.dat
			echo "Listo"
		;;

		"0")
			echo "Saliendo"
		;;

		* ) 	#Esto es por defecto. Vacío entra aquí tb.

			echo "OPCION INCORRECTA."
		;;
	esac	#Fin case.
done

exit 0
