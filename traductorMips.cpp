#include <map>
#include <vector>
#include <string>

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
