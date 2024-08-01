/*
system is made up of:

Memory of 0xFFF bytes
16 8-Bit data registers, called V0 to VF. VF doubles as the carry flag
16 Bit address register which is used to access memory
16 Bit program counter and a stack
*/
#include <vector>

typedef unsigned char BYTE; //8bit
typedef unsigned short int WORD; //16bit

BYTE m_GameMemory[0xFFF] ; // 0xFFF bytes of memory
BYTE m_Registers[16] ; // 16 registers, 1 byte each
WORD m_AddressI ; // the 16-bit address register
WORD m_ProgramCounter ; // the 16-bit program counter
std::vector m_Stack; // the 16-bit stack

BYTE m_ScreenData[64][32];

void CPUReset() {
   m_AddressI = 0 ;
   m_ProgramCounter = 0x200 ;
   memset(m_Registers,0,sizeof(m_Registers)) ; // setting registers to 0

   // load in the game
   FILE *in;
   in = fopen( "c:/INVADERS", "rb" ); //game location
   fread( &m_GameMemory[0x200], 0xfff, 1, in); 
   fclose(in);
}

WORD GetNextOpcode() {
   WORD res = 0 ; //0000000000000000
   res = m_GameMemory[m_ProgramCounter] ; 
   res <<= 8 ;
   res |= m_GameMemory[m_ProgramCounter+1] ;
   m_ProgramCounter+=2 ; //add 2 as opcode is 2 bytes
   return res ;
}