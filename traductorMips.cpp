#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <bitset>

using namespace std;

// Mapeo de registros
map<string, string> registrosBinario = {
    {"$zero", "00000"}, {"$at", "00001"}, {"$v0", "00010"}, {"$v1", "00011"},
    {"$a0", "00100"}, {"$a1", "00101"}, {"$a2", "00110"}, {"$a3", "00111"},
    {"$t0", "01000"}, {"$t1", "01001"}, {"$t2", "01010"}, {"$t3", "01011"},
    {"$t4", "01100"}, {"$t5", "01101"}, {"$t6", "01110"}, {"$t7", "01111"},
    {"$s0", "10000"}, {"$s1", "10001"}, {"$s2", "10010"}, {"$s3", "10011"},
    {"$s4", "10100"}, {"$s5", "10101"}, {"$s6", "10110"}, {"$s7", "10111"},
    {"$t8", "11000"}, {"$t9", "11001"}, {"$k0", "11010"}, {"$k1", "11011"},
    {"$gp", "11100"}, {"$sp", "11101"}, {"$fp", "11110"}, {"$ra", "11111"}
};


// Mapa de instrucciones con metadatos binarios
map<string, vector<int>> instrucciones = {
    /* Formato guia: {"instruccion",{tipo (0=R ,1=I ,2=J), opcode (binario), funct (binario), cant_operandos,
                        posicion rs, pos rt, pos rd, pos shamt o inmediato}} */

    // Tipo R
    {"add",   {0, 000000, 100000, 3, 1, 2, 0, -1}},
    {"sub",   {0, 000000, 100010, 3, 1, 2, 0, -1}},
    {"addu",  {0, 000000, 100001, 3, 1, 2, 0, -1}},
    {"and",   {0, 000000, 100100, 3, 1, 2, 0, -1}},
    {"or",    {0, 000000, 100101, 3, 1, 2, 0, -1}},
    {"subu",  {0, 000000, 100011, 3, 1, 2, 0, -1}},
    {"mult",  {0, 000000, 011000, 2, 0, 1, -1, -1}},
    {"multu", {0, 000000, 011001, 2, 0, 1, -1, -1}},
    {"div",   {0, 000000, 011010, 2, 0, 1, -1, -1}},
    {"mfhi",  {0, 000000, 010000, 1, -1, -1, 0, -1}},
    {"mflo",  {0, 000000, 010010, 1, -1, -1, 0, -1}},
    {"sll",   {0, 000000, 000000, 3, -1, 1, 0, 2}},
    {"srl",   {0, 000000, 000010, 3, -1, 1, 0, 2}},
    {"jr",    {0, 000000, 001000, 1, 0, -1, -1, -1}},
    {"slt",   {0, 000000, 101010, 3, 1, 2, 0, -1}},
    {"sltu",  {0, 000000, 101011, 3, 1, 2, 0, -1}},

    // Tipo I
    {"addi",  {1, 001000, -1, 3, 1, 0, -1, 2}},
    {"addiu", {1, 001001, -1, 3, 1, 0, -1, 2}},
    {"lw",    {1, 100011, -1, 2, 2, 0, -1, 1}},
    {"sw",    {1, 101011, -1, 2, 2, 0, -1, 1}},
    {"lui",   {1, 001111, -1, 2, -1, 0, -1, 1}},
    {"andi",  {1, 001100, -1, 3, 1, 0, -1, 2}},
    {"ori",   {1, 001101, -1, 3, 1, 0, -1, 2}},
    {"slti",  {1, 001010, -1, 3, 1, 0, -1, 2}},
    {"sltiu", {1, 001011, -1, 3, 1, 0, -1, 2}},
    {"beq",   {1, 000100, -1, 3, 0, 1, -1, 2}},

    // Tipo J
    {"j",     {2, 000010, -1, 1, -1, -1, -1, 0}},
    {"jal",   {2, 000011, -1, 1, -1, -1, -1, 0}}
};


map<string, vector<string>> tiposInstrucciones = {
    {"add",   {"registro", "registro", "registro"}},    // add rd, rs, rt
    {"sub",   {"registro", "registro", "registro"}},    // sub rd, rs, rt
    {"addi",  {"registro", "registro", "immediato"}},    // addi rt, rs, imm
    {"addu",  {"registro", "registro", "registro"}},    // addu rd, rs, rt
    {"subu",  {"registro", "registro", "registro"}},    // subu rd, rs, rt
    {"addiu", {"registro", "registro", "immediato"}},    // addiu rt, rs, imm

    {"mfc0",  {"registro", "cop0"}},          // mfc0 rt, rd (de coprocesador 0) NO ENTIENDO ESTO como asi cop0

    {"mult",  {"registro", "registro"}},           // mult rs, rt
    {"multu", {"registro", "registro"}},           // multu rs, rt
    {"div",   {"registro", "registro"}},           // div rs, rt
    {"mfhi",  {"registro"}},                 // mfhi rd
    {"mflo",  {"registro"}},                 // mflo rd
    {"lw",    {"registro", "offset"}},       // lw rt, offset(rs)
    {"sw",    {"registro", "offset"}},       // sw rt, offset(rs)
    {"lhu",   {"registro", "offset"}},       // lhu rt, offset(rs)
    {"sh",    {"registro", "offset"}},       // sh rt, offset(rs)
    {"lbu",   {"registro", "offset"}},       // lbu rt, offset(rs)
    {"sb",    {"registro", "offset"}},       // sb rt, offset(rs)
    {"ll",    {"registro", "offset"}},       // ll rt, offset(rs)
    {"sc",    {"registro", "offset"}},       // sc rt, offset(rs)
    {"lui",   {"registro", "immediato"}},          // lui rt, imm
    {"AND",   {"registro", "registro", "registro"}},    // AND rd, rs, rt
    {"OR",    {"registro", "registro", "registro"}},    // OR rd, rs, rt
    {"ANDi",  {"registro", "registro", "immediato"}},    // ANDi rt, rs, imm
    {"ORi",   {"registro", "registro", "immediato"}},    // ORi rt, rs, imm
    {"sll",   {"registro", "registro", "immediato"}},    // sll rd, rt, imm
    {"srl",   {"registro", "registro", "immediato"}},    // srl rd, rt, imm
    {"beq",   {"registro", "registro", "label"}},  // beq rs, rt, label
    {"slt",   {"registro", "registro", "registro"}},    // slt rd, rs, rt
    {"slti",  {"registro", "registro", "immediato"}},    // slti rt, rs, imm
    {"sltu",  {"registro", "registro", "registro"}},    // sltu rd, rs, rt
    {"sltiu", {"registro", "registro", "immediato"}},    // sltiu rt, rs, imm
    {"j",     {"etiqueta"}},               // j label
    {"jr",    {"registro"}},                 // jr rs
    {"jal",   {"etiqueta"}}                // jal label
};

map<string, int> etiquetas  = {};
vector<vector<string>> MIPS;


// string convertirTipoI(vector<string>) {
// 
// }



vector<string> quitarComas(string s) {
    vector<string> resultado;
    stringstream ss(s);
    string temp;

    while (getline(ss, temp, ',')) {
        resultado.push_back(temp);
    }

    return resultado;
}



bool esRegistro(const string& operando) {
    return registrosBinario.find(operando) != registrosBinario.end();
}

bool esNumero(const string& operando) {
    bool valido = !operando.empty();
    for (int i = 0; i < operando.size() && valido; i++) {
        valido = isdigit(operando[i]);
    }
    return valido;
}

bool esDireccionMemoria(const string& operando) {
    bool valido = operando.size() >= 2 && operando[0] == '[' && operando.back() == ']';
    return valido;
}

bool esTipoValido(const string& operando, const string& tipoEsperado) {
    bool valido = false;

    if (tipoEsperado == "registro") {
        valido = esRegistro(operando);
    }
    if (tipoEsperado == "inmediato") {                // PUEDE SER DECIMAL O HEXA
        valido = esNumero(operando);
    }
    if (tipoEsperado == "offset") {                  // DIR EN EL FORMATO 4($s3)
        valido = esDireccionMemoria(operando);
    }

    return valido;
}



bool esInstruccionValida(int contador, string instruccion) {
    bool ans = true;
    string etiqueta = "";

    vector<string> instruccioness = quitarComas(instruccion);
    if (instrucciones.size() == 1 && instrucciones[0].back() == ':') {
        etiquetas[instruccioness[0]] = contador + 0x0004;
    }
    else {
        if (instrucciones.find(instruccioness[0]) != instrucciones.end()) {
            if (instruccioness.size() - 1 != instrucciones[instruccioness[0]][3]) {
                ans = false;
            }
            else {
                for (int i = 0; i < instrucciones[instruccioness[0]][3] && ans; i++) {
                    ans = esTipoValido(instruccioness[i+1],tiposInstrucciones[instruccioness[0]][i+1]);
                }
            }

        }
        else {
            ans = false;
        }
    }

    if (ans) {
        MIPS.push_back(instruccioness);
    }
    return ans;
}

int archivoValido() {

    int contador = 0x0000;

    ifstream archivo("archivo.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    string linea;
    bool valido = true;
    while (getline(archivo, linea) && valido) {
        valido = esInstruccionValida(contador, linea);
        contador += 0x0004;
    }

    if (!valido) {
        contador = -1;
    }

    return contador;
}
string convertirTipoI(vector<string> instruccion) {
    string opcode = bitset<6>(instrucciones[instruccion[0]][1]).to_string();
    string rs = registrosBinario[instruccion[instrucciones[instruccion[0]][4] + 1]];
    string rt = registrosBinario[instruccion[instrucciones[instruccion[0]][5] + 1]];
    string immediate = bitset<16>(stoi(instruccion[instrucciones[instruccion[0]][7] + 1])).to_string();
    return opcode + rs + rt + immediate;
}

string convertirTipoJ(vector<string> instruccion) {
    string opcode = bitset<6>(instrucciones[instruccion[0]][1]).to_string();
    string address = bitset<26>(etiquetas[instruccion[1]]).to_string();
    return opcode + address;
}

string convertirTipoR(vector<string> instruccion) {
    string opcode = bitset<6>(instrucciones[instruccion[0]][1]).to_string();
    string rs = registrosBinario[instruccion[instrucciones[instruccion[0]][4] + 1]];
    string rt = registrosBinario[instruccion[instrucciones[instruccion[0]][5] + 1]];
    string rd = registrosBinario[instruccion[instrucciones[instruccion[0]][6] + 1]];
    string shamt = bitset<5>(instrucciones[instruccion[0]][7] == -1 ? 0 : stoi(instruccion[instrucciones[instruccion[0]][7] + 1])).to_string();
    string funct = bitset<6>(instrucciones[instruccion[0]][2]).to_string();
    return opcode + rs + rt + rd + shamt + funct;
}

string convertirInstruccion(vector<string> instruccion) {
    int tipo = instrucciones[instruccion[0]][0];
    if (tipo == 0) {
        return convertirTipoR(instruccion);
    } else if (tipo == 1) {
        return convertirTipoI(instruccion);
    } else if (tipo == 2) {
        return convertirTipoJ(instruccion);
    }
    return "";
}

int main() {
    // Código para leer el archivo y validar las instrucciones
    int contador = archivoValido();
    if (contador == -1) {
        cout << "Archivo no valido." << endl;
        return 1;
    }

    // Convertir las instrucciones a binario
    for (const auto& instruccion : MIPS) {
        string binario = convertirInstruccion(instruccion);
        cout << binario << endl;
    }

    // Test cases
    vector<string> testInstruccion1 = {"addi", "$t0", "$t1", "10"};
    vector<string> testInstruccion2 = {"j", "etiqueta"};
    vector<string> testInstruccion3 = {"add", "$t0", "$t1", "$t2"};
    vector<string> testInstruccion4 = {"sub", "$s0", "$s1", "$s2"};
    vector<string> testInstruccion5 = {"lw", "$t0", "4($s3)"};
    vector<string> testInstruccion6 = {"sw", "$t1", "8($s4)"};
    vector<string> testInstruccion7 = {"beq", "$t0", "$t1", "label"};
    vector<string> testInstruccion8 = {"ori", "$t2", "$t3", "15"};
    vector<string> testInstruccion9 = {"sll", "$t4", "$t5", "2"};
    vector<string> testInstruccion10 = {"jr", "$ra"};

    cout << "Test ADDI: " << convertirInstruccion(testInstruccion1) << " (Expected: 00100001001010000000000000001010)" << endl;
    cout << "Test J: " << convertirInstruccion(testInstruccion2) << " (Expected: 00001000000000000000000000000000)" << endl;
    cout << "Test ADD: " << convertirInstruccion(testInstruccion3) << " (Expected: 00000001001010100100000000100000)" << endl;
    cout << "Test SUB: " << convertirInstruccion(testInstruccion4) << " (Expected: 00000010001100011001000000100010)" << endl;
    cout << "Test LW: " << convertirInstruccion(testInstruccion5) << " (Expected: 10001110011010000000000000000100)" << endl; // nose por que no funciona
    cout << "Test SW: " << convertirInstruccion(testInstruccion6) << " (Expected: 10101110100101010000000000001000)" << endl;
    cout << "Test BEQ: " << convertirInstruccion(testInstruccion7) << " (Expected: 00010001000010010000000000000000)" << endl;
    cout << "Test ORI: " << convertirInstruccion(testInstruccion8) << " (Expected: 00110101011010100000000000001111)" << endl;
    cout << "Test SLL: " << convertirInstruccion(testInstruccion9) << " (Expected: 00000000000010100101000001000000)" << endl;
    cout << "Test JR: " << convertirInstruccion(testInstruccion10) << " (Expected: 00000011111000000000000000001000)" << endl;

    return 0;
}
