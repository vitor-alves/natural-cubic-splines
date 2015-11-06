# compilar
buildSpline: main.c spline.c
	gcc main.c spline.c -o spline
# deletar executável e arquivo saída
clean:
	rm -f spline splines.txt
