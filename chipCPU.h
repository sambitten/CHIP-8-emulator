#pragma once
#ifndef CHIPCPU_H_INCLUDED
#define CHIPCPU_H_INCLUDED

#include <string>
#include <vector>

typedef unsigned char BYTE ;
typedef unsigned short int WORD ;

const int ROMSIZE = 0xFFF ;

class ChipCPU
{

  public:
    ~ChipCPU();
    static ChipCPU* CreateSingleton();
    
	void ExecuteNextOpcode();

	BYTE m_ScreenData[320][640][3] ; 

  private:
    ChipCPU();

	WORD GetNextOpcode();
    

    void CPUReset();
	void ClearScreen();
    void disp_clear();

    void Opcode0(WORD opcode);
    void Opcode00EE();


    static ChipCPU* m_Instance;

    BYTE m_GameMemory[ROMSIZE]; 
    BYTE m_Registers[16];
    WORD m_AddressI;
    WORD m_ProgramCounter;
	std::vector<WORD> m_Stack;
			
};


#endif // CHIPCPU_H_INCLUDED
