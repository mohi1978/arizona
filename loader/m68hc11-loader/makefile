# Programmer le 68HC11 sous Linux. 
#
# Le programme est en deux parties : 
# 1 - le code d’initialisation du port serie d’une part (rs232.c) 
# 2 et le code de lecture du fichier contenant le programme `a charger en hexadecimal et  de  communication  avec  le  HC11  (hc11linux.c).  
# La compilation  se  fait  par :

all: rs232.o hc11_linux.o hc11_linux hc11rec hc11_ee64 

rs232.o: rs232.c
	gcc -c rs232.c

hc11_linux.o: hc11_linux.c
	gcc -c hc11_linux.c

hc11_linux:
	gcc -o hc11_linux hc11_linux.c rs232.o

hc11rec:
	gcc -o hc11rec hc11rec.c rs232.o

hc11_ee64:
	gcc -o hc11_ee64 hc11_ee64.c rs232.o

clean:
	rm *.o hc11_linux hc11rec hc11_ee64
