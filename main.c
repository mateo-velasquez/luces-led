#include <stdio.h> //Permite salida de datos e info en C
#include <stdlib.h> //Gestiona la Mermoria (Creo que podría ser sacable, pero la tenía el profe)
#include <termios.h> //Proporcionará la interfaz para configurar la terminal
#include <unistd.h> //Proporciona acceso a la API del Sistema Operativo POSIX
#include <stdbool.h> //Introduce el tipo de dato Booleano en C
#include <fcntl.h> //Se va a usar en la pulsación de las teclas para cambiar el modo de los descriptores de archivo
//MODO NO BLOQUEANTE EN LA TERMINAL
#include <ncurses.h> //API para la manipulación de la pantalla y el teclado (Podría ser sacable)
#include "EasyPIO.h" //Manejamos los Pines GPIO de la Raspberry
#include "assembly.h" //RENOMBRAR

#define LONGITUD_PASSWORD 5

void disp_binary(int); //muestra las luces por pantalla

void verificarPassword(char *password); //verifica la contraseña

void menu(); //muestra el menu

void configurarPines(void); //configuración con los leds

void mostrarLuces(unsigned char);//Muestra un patrón en los LEDs basado en el valor de output.

void apagarLeds();//Apaga todos los LEDs.

void autoFantastico(); //auto fantastico

void choque(); //choque

//void BarraCarga();

//void Pendulo();

struct termios modificarTerminal(void); 
//Modifica la configuración del terminal para desactivar el modo canónico y el eco.

void restaurarTerminal(struct termios); //Restaura la configuración del terminal a su estado original.

bool teclas(int index); //Verifica si una tecla ha sido presionada y 
//ajusta la velocidad de los efectos de luz basándose en las teclas presionadas (W o S).

int retraso(int index);//Introduce un retraso basado en el valor de tiempoRetraso[index], 
//verificando periódicamente si una tecla ha sido presionada.

void limpiarBuffer(); //Limpia el búfer de entrada.

const unsigned char led[] = {14, 15, 18, 23, 24, 25, 8, 7}; //Arreglo con los números de designación
//Elegí conectar los Leds a los números de designación que le da BCM
int tiempoRetraso[] = {10000, 10000, 10000, 10000};

int main(void) {
    configurarPines();
    char Password[5] = {'1', '2', '3', '4', '5'};
    char passwordInput[5];

    //Recibe la contraseña y la verifica
    //Si la contraseña se ingresa tres veces mal, se aborta el inicio
    for (int i = 0; i < 3; i++) {
        bool bandera = true;
        verificarPassword(passwordInput);

        for (int j = 0; j < 5; j++) {
            if (Password[j] != passwordInput[j]) {//compara char a char si es o no la contraseña correcta
                bandera = false;
                break;
            }
        }

        if (bandera) {
            printf("Bienvenido al sistema !\n\n");
            menu();
            printf("Hasta luego !\n");
            break;
        } else {
            printf("Clave incorrecta\n\n");
        }

    }
}

void disp_binary(int i) {
    int t;
    for (t = 128; t > 0; t = t / 2)
        if (i & t) printf('*');
        else printf('_');
    fflush(stdout);
    printf("\r");
}

void verificarPassword(char *password) {

    struct termios viejosAtr = modificarTerminal(); //modifica la terminal
    printf("Ingrese su password: "); 
    for (int i = 0; i < LONGITUD_PASSWORD; i++) {// Lee la password
        password[i] = getchar();
        printf("*"); // imprime el asterisco
        fflush(stdout);
    }
    restaurarTerminal(viejosAtr); //restaura la terminal
    printf("\n");
}

void menu() {
    int opcion;

    do {
        limpiarBuffer(); //limpio el buffer
        printf("Seleccione una opcion:\n");
        printf("1: Auto Fantastico\n");
        printf("2: El Choque\n");
        printf("3: Barra de Carga\n");
        printf("4: Pendulo\n");
        printf("0: Salir\n");
        scanf("%d", &opcion); //función del stdio para obtener la opción

        switch (opcion) {
            case 1:
                autoFantastico();
                break;
            case 2:
                choque();
                break;
            case 3:
                BarraCarga();
                break;
            case 4:
                Pendulo();
                break;
            case 0:
                break;
            default:
                printf("Seleccione una opcion valida\n");
        }
    } while (opcion != 0);
}

void configurarPines(void) {
    pioInit(); //inicializa para usar EasyPIO
    for (int i = 0; i < 8; i++) {
        pinMode(led[i], OUTPUT);//Define si es entrada o salida
    }//claramente van a ser los 8 salidas

}

void mostrarLuces(unsigned char output) {
    for (int j = 0; j < 8; j++) {
        digitalWrite(led[j], (output >> j) & 1);// Escribe a un puerto
    }
}

void apagarLeds() {
    unsigned char apagado = 0x00; //es un hexa de 0000 0000
    mostrarLuces(apagado);

}

void autoFantastico() {
    printf("Presione esc para finalizar la secuencia\n");
    printf("Presione + para aumentar la velocidad\n");
    printf("Presione - para disminuir la velocidad\n");
    printf("Auto Fantastico:\n");

    unsigned char tabla[15] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

    while (true) {
        for (int i = 0; i < 15; i++) {
            mostrarLuces(tabla[i]);
            disp_binary(tabla[i]);

            if (retraso(0) == 0) {
                apagarLeds();
                return;
            }
        }
    }
}

void choque() {
    printf("Presione esc para finalizar la secuencia\n");
    printf("Presione + para aumentar la velocidad\n");
    printf("Presione - para disminuir la velocidad\n");
    printf("Choque:\n");

    unsigned char output, aux1, aux2;

    while (true) {
        aux1 = 0x80;
        aux2 = 0x1;
        for (int i = 0; i < 7; i++) {
            output = aux1 | aux2;
            mostrarLuces(output);
            disp_binary(output);
            aux1 = aux1 >> 1;
            aux2 = aux2 << 1;
            if (retraso(1) == 0) {
                apagarLeds();
                return;
            }
        }
    }
}

/*void BarraCarga() {
    printf("Presione esc para finalizar la secuencia\n");
    printf("Presione + para aumentar la velocidad\n");
    printf("Presione - para disminuir la velocidad\n");
    printf("BarraCarga:\n");

    unsigned char led = 0x80;

    while (true) {
        for (int i=0; i<8; i++){
            mostrarLuces(led);
            disp_binary(led);        
            led = led >> 1;

            if (retraso(0) == 0) {
                apagarLeds();
                return;
            }
        }
    }

}
*/

/*void Pendulo() {
    printf("Presione esc para finalizar la secuencia\n");
    printf("Presione + para aumentar la velocidad\n");
    printf("Presione - para disminuir la velocidad\n");
    printf("Pendulo:\n");

    unsigned char tabla[14] = {0x88, 0x48, 0x28, 0x18, 0x14, 0x12, 0x11, 0x11, 0x12, 0x14, 0x18, 0x28, 0x48, 0x88};

    while (true) {
        for (int i = 0; i < 14; i++) {
            mostrarLuces(tabla[i]);
            disp_binary(tabla[i]);

            if (retraso(0) == 0) {
                apagarLeds();
                return;
            }
        }
    }

}
*/

struct termios modificarTerminal(void) {
    struct termios viejosAtr, nuevosAtr;
    tcgetattr(STDIN_FILENO, &viejosAtr);// Obtiene los atributos de la terminal actual   
    nuevosAtr = viejosAtr; // Copia los atributos actuales a los nuevos atributos.
    // Desactivar el modo canónico y el eco.
    nuevosAtr.c_lflag &= ~(ICANON | ECHO); //Controla varias funciones de terminal.
    tcsetattr(STDIN_FILENO, TCSANOW, &nuevosAtr);//Aplicar los nuevos atributos a la terminal.
    return viejosAtr;
}

void restaurarTerminal(struct termios viejosAtr) {
    // Restaura los atributos originales del terminal.
    tcsetattr(STDIN_FILENO, TCSANOW, &viejosAtr);
}

bool teclas(int index) {

    struct termios viejosAtr = modificarTerminal();
    int car, oldf;
    //Establece el descriptor de archivo de la entrada estándar en modo sin bloqueo
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    // Intenta leer un carácter de la entrada estándar
    car = getchar();

    //Lee el caracter +
    if(car == 43) {
        if(tiempoRetraso[index] > 500){
            tiempoRetraso[index] = tiempoRetraso[index] - 500;
        }
    }

    //lee el caracter -
    if(car == 45) {
        if(tiempoRetraso[index] < 11000){
            tiempoRetraso[index] = tiempoRetraso[index] + 500;
        }
    }
    restaurarTerminal(viejosAtr);

    // Restaura el modo de descriptor de archivo
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    // Si se presiona la tecla esc, devuelve 1
    if (car == 27) {
        ungetc(car, stdin);
        return 1;
    }
    // Si no se registró un esc, devuelve 0
    return 0;
}

int retraso(int index)
{
    int i;
    unsigned int j;
    for(i=tiempoRetraso[index]; i > 0; --i) 
    // repite el número especificado de veces 
        if(teclas(index)) {
            return 0;
        }
    return 1;
}

void limpiarBuffer() {
    printf("Presione ENTER para confirmar\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // Esta función se asegura de que cualquier entrada restante en el búfer se descarte hasta que se presione ENTER.
    }
}


/*
BIBLIOGRAFÍA

https://www.programoergosum.es/tutoriales/introduccion-a-pines-gpio-en-raspbian/
https://baulderasec.wordpress.com/programando-2/programacion-con-linux/5-terminales/estructura-termios/modos-locales/
https://www.ibm.com/docs/es/aix/7.3?topic=files-termiosh-file
*/