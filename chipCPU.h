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

    void KeyPressed(int key);
		void KeyReleased(int key);
	  void ExecuteNextOpcode();

	  BYTE m_ScreenData[320][640][3] ; 

  private:
    ChipCPU();
	  WORD GetNextOpcode();
    void CPUReset();
    void Disp_clear();
    void PlayBeep();
		int	GetKeyPressed();
    
    void Opcode0(WORD opcode);
    void Opcode00EE();
    void Opcode1(WORD opcode);
    void Opcode2(WORD opcode);
    void Opcode3(WORD opcode);
    void Opcode4(WORD opcode);
    void Opcode5(WORD opcode);
    void Opcode6(WORD opcode);
    void Opcode7(WORD opcode);
    void Opcode8(WORD opcode);
    void Opcode8XY0(WORD opcode);
		void Opcode8XY1(WORD opcode);
		void Opcode8XY2(WORD opcode); 
		void Opcode8XY3(WORD opcode);
		void Opcode8XY4(WORD opcode); 
		void Opcode8XY5(WORD opcode); 
		void Opcode8XY6(WORD opcode);
		void Opcode8XY7(WORD opcode);
		void Opcode8XYE(WORD opcode);
    void Opcode9(WORD opcode);
    void OpcodeA(WORD opcode);
    void OpcodeB(WORD opcode);
    void OpcodeC(WORD opcode);
    void OpcodeD(WORD opcode);
    void OpcodeE(WORD opcode);
    void OpcodeEX9E(WORD opcode);
		void OpcodeEXA1(WORD opcode);
    void OpcodeF(WORD opcode);
    void OpcodeFX07(WORD opcode); 
		void OpcodeFX0A(WORD opcode); 
		void OpcodeFX15(WORD opcode);
		void OpcodeFX18(WORD opcode);
		void OpcodeFX1E(WORD opcode); 
		void OpcodeFX29(WORD opcode); 
		void OpcodeFX33(WORD opcode); 
		void OpcodeFX55(WORD opcode); 
		void OpcodeFX65(WORD opcode);

    static ChipCPU* m_Instance;

    BYTE m_GameMemory[ROMSIZE]; 
    BYTE m_Registers[16];
    WORD m_AddressI;
    WORD m_ProgramCounter;
	  std::vector<WORD> m_Stack;
    BYTE m_KeyState[16];
    BYTE m_DelayTimer;
		BYTE m_SoundTimer;
};


#endif // CHIPCPU_H_INCLUDED