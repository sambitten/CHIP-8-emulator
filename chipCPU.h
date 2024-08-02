#pragma once
#ifndef CHIP8CPU_H_INCLUDED
#define CHIP8CPU_H_INCLUDED

#include <string>
#include <vector>

typedef unsigned char BYTE ;
typedef unsigned short int WORD ;

const int ROMSIZE = 0xFFF ;

class Chip8CPU
{

  public:
    ~Chip8CPU();
    static Chip8CPU* CreateSingleton();
    
	void ExecuteNextOpcode();

	BYTE m_ScreenData[320][640][3] ; 



  private:
    Chip8CPU();

	WORD GetNextOpcode();

    void CPUReset();
	void ClearScreen();

    static Chip8CPU* m_Instance;

    BYTE m_GameMemory[ROMSIZE];
    BYTE m_Registers[16];
    WORD m_AddressI;
    WORD m_ProgramCounter;

	std::vector<WORD> m_Stack;
			
};


#endif // CHIP8CPU_H_INCLUDED
