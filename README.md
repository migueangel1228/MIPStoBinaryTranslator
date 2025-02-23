# MIPStoBinaryTranslator
## Práctica primer parcial
El objetivo de la parte práctica, es afianzar el conocimiento sobre el ISA y las arquitecturas de cómputo MIPS. Para lograr este objetivo, el estudiante debe desarrollar un programa en alto nivel C++ que permita cargar un archivo .txt, que contenga un algoritmo en lenguaje assembly de MIPS. 
El programa debe validar si el algoritmo está en el lenguaje assembly de MIPS
y cumple con la sintaxis de las instrucciones. En caso de que no cumpla, se debe
generar un mensaje al usuario indicando que no cumple con los requisitos de la
sintaxis.
Es importante tener en cuenta que no se puede modificar el registro zero, el
registro gp y los registros del kernel.
El siguiente paso, después de la validación, es la traducción del programa a
lenguaje binario. Se deben calcular las direcciones de las etiquetas en los Branch
y los Jump (j y jal). Si las direcciones calculadas para los saltos branch resultan
negativas, se deben poner en complemento a dos en la traducción.
Finalmente, se debe generar un archivo de salida .txt donde se encuentra el
algoritmo en binario. No olvide incluir las instrucciones Mult, Div, mflo, mfhi,
sra.
