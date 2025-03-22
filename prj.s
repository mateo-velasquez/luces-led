//assembly
.text

.global BarraCarga
BarraCarga:
        PUSH {R4,R5,LR} //Salvo los registros, inserto en la pila (demanda la convensión)
        //también salvo al Link register porque tiene la dirección de retorno, porque llamo a otras subrutinas

resetB:
        MOV R4,#7 //inicializamos el Registro en 7, por que es nuestro contador.
        MOV R5,#0x80 //al registro 5 le asignamos 10000000 (binario)

        //Aca empieza el bucle loopB:
loopB:
        MOV R0,R5 //Al registro R0 le asigno lo que tiene el registro R5
        BL disp_binary //Llamo a la función disp_binary en C

        MOV R0,R5 //Al registro R0 le asigno lo que tiene el registro R5
        BL mostrarLuces //Llamo a la función mostrarLuces en C
        
        ASR R5,R5,#1 //Aritmethic Shift right, para hacer un corrimiento aritmético al registro 5
        
        MOV R0,#2 //Index para el arreglo que está en el código en C
        BL retraso //Llamo a la función retraso en C

        SUBS R4,R4,#1 //Decremento en 1 lo que esté en R4 y activo las banderas
        BEQ resetB //Branch Equal (si R4 == 0, entonces reinicia las variables)

        CMP R0,#0 // Compara el resultado de retraso y activo las banderas
        BNE loopB //Branch not equal (si el resultado de retraso es distinto de 0, vuelve a loopB)
        BL apagarLeds //apaga las leds

        POP {R4,R5,PC} //me ahorro el MOV PC, LR

.global Pendulo
Pendulo:
        PUSH {R4,R5,R6,LR} // Salvo los registros

reset:
        MOV R4,#14 // Aca inicializamos en 14 porque va a ser el contador y nuestro array es de 14 bytes
        LDR R5, =pend // carga en el registro 5 la dirección del arreglo (va a acceder al pend[0]) (pseudoinstrucción cargar hexa)

    //Aca empieza el loop (bucle)
loop:   
        LDRB R6,[R5],#1 //hace un incremento post indexado
        
        MOV R0,R6 //Asigna lo que está en el registro 6 al Registro 0
        BL disp_binary //Llama a la función disp_binary con lo de R0
        
        MOV R0,R6 //Asigna lo que está en el registro 6 al Registro 0
        BL mostrarLuces //Llama a la función mostrarLuces con lo de R0
        
        MOV R0,#3 //Index para el arreglo que está en el código en C
        BL retraso //Llamo a la función retraso en C

        SUBS R4,R4,#1 //Decremento en 1 lo que esté en R4 y activo las banderas
        BEQ reset //Branch Equal (si R4 == 0, entonces reinicia las variables)

        CMP R0,#0 // Compara el resultado de retraso y activo las banderas
        BNE loop //Si no es igual, entonces vuelve a loop
        BL apagarLeds //apaga las leds

        POP {R4,R5,R6,PC}

.data
//Aca simplemente declaro el arreglo con los valores en hexadecimal que representan las leds
pend: 
    .byte 0x88
    .byte 0x48
    .byte 0x28
    .byte 0x18
    .byte 0x14
    .byte 0x12
    .byte 0x11
    .byte 0x12
    .byte 0x14
    .byte 0x18
    .byte 0x28
    .byte 0x48
    .byte 0x48
    .byte 0x88

.end