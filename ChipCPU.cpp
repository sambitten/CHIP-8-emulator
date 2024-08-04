#include "ChipCPU.h"
#include <assert.h>

ChipCPU* ChipCPU::m_Instance = 0 ;

ChipCPU* ChipCPU::CreateSingleton(){
    if (0 == m_Instance)
        m_Instance = new ChipCPU( ) ;
    return m_Instance ;
}


ChipCPU::ChipCPU(){}
ChipCPU::~ChipCPU(){}

void ChipCPU::CPUReset() {
   m_AddressI = 0 ;
   m_ProgramCounter = 0x200 ;
   memset(m_Registers,0,sizeof(m_Registers)) ; // setting registers to 0

   // load in the game
   FILE *in;
   in = fopen( "c:/INVADERS", "rb" ); //game location
   fread( &m_GameMemory[0x200], 0xfff, 1, in); 
   fclose(in);
}

WORD ChipCPU::GetNextOpcode() {
   WORD res = 0 ; //0000000000000000
   res = m_GameMemory[m_ProgramCounter] ; 
   res <<= 8 ;
   res |= m_GameMemory[m_ProgramCounter+1] ;
   m_ProgramCounter+=2 ; //add 2 as opcode is 2 bytes
   return res ;
}


/*
Using the opcode table found on the chip8 wiki,
Take the first hex value in the opcode to call relevent function to decode opcode,
Some cases will need to be broken down further with more switch statements in further functions
*/
void ChipCPU::ExecuteNextOpcode() {
	WORD opcode = GetNextOpcode();
   /*
	switch (opcode & 0xF000){

		case 0x0000: Opcode0(opcode) ; break ;
		case 0x1000: Opcode1(opcode) ; break ;
		case 0x2000: Opcode2(opcode) ; break ;
		case 0x3000: Opcode3(opcode) ; break ;
		case 0x4000: Opcode4(opcode) ; break ;
		case 0x5000: Opcode5(opcode) ; break ;
		case 0x6000: Opcode6(opcode) ; break ;
		case 0x7000: Opcode7(opcode) ; break ;
		case 0x8000: Opcode8(opcode) ; break ;
		case 0x9000: Opcode9(opcode) ; break ;
		case 0xA000: OpcodeA(opcode) ; break ;
		case 0xB000: OpcodeB(opcode) ; break ;
		case 0xC000: OpcodeC(opcode) ; break ;
		case 0xD000: OpcodeD(opcode) ; break ;
		case 0xE000: OpcodeE(opcode) ; break ;
		case 0xF000: OpcodeF(opcode) ; break ;

		default: break ;
	}
   */
}

void ChipCPU::Opcode0(WORD opcode){

	switch (opcode & 0x000F){

		case 0x0000:disp_clear();
		case 0x000E:Opcode00EE();
	
	default: break;
	}
}

void ChipCPU::disp_clear(){
	//clear screen
}

void ChipCPU::Opcode00EE()
{
	m_ProgramCounter = m_Stack.back( ) ;
	m_Stack.pop_back( ) ;
}