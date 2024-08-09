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

void ChipCPU::Disp_clear(){
	for (int x = 0; x < 640; x++){
		for (int y = 0 ; y < 320; y++){
			m_ScreenData[y][x][0] = 255 ;
			m_ScreenData[y][x][1] = 255 ;
			m_ScreenData[y][x][2] = 255 ;
		}
	}
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

		case 0x0000:Disp_clear();
		case 0x000E:Opcode00EE();
	
	default: break;
	}
}

void ChipCPU::Opcode00EE(){
	m_ProgramCounter = m_Stack.back();
	m_Stack.pop_back();
}

// jump to address 
void ChipCPU::Opcode1(WORD opcode){
	m_ProgramCounter = opcode & 0x0FFF;
}

// call subroutine 
void ChipCPU::Opcode2(WORD opcode){
	m_Stack.push_back(m_ProgramCounter);
	m_ProgramCounter = opcode & 0x0FFF;
}

// skip next instruction if VX == NN
void ChipCPU::Opcode3(WORD opcode){
	int nn = opcode & 0x00FF ;
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	if (m_Registers[regx] == nn)
		m_ProgramCounter += 2 ;
}

// skip next instruction if VX != NN
void ChipCPU::Opcode4(WORD opcode){
	int nn = opcode & 0x00FF ;
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	if (m_Registers[regx] != nn)
		m_ProgramCounter += 2 ;
}

// skip next instruction if VX == VY
void ChipCPU::Opcode5(WORD opcode){
	int regx = opcode & 0x0F00 ;
	int regy = opcode & 0x00F0 ;
	regx >>= 8;
	regy >>= 4;

	if (m_Registers[regx] == m_Registers[regy])
		m_ProgramCounter += 2 ;
}

// set VX = NN
void ChipCPU::Opcode6(WORD opcode){
	int nn = opcode & 0x00FF ;
	int regx = opcode & 0x0F00 ;
	regx >>= 8;

	m_Registers[regx] = nn;
}

// adds NN to vx. carry not affected
void ChipCPU::Opcode7(WORD opcode){
	int nn = opcode & 0x00FF ;
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	m_Registers[regx] += nn;
}

// opcode 8 needs to be broken down further
void ChipCPU::Opcode8(WORD opcode){
	
	switch (opcode & 0xF)
	{
		case 0x0: Opcode8XY0(opcode) ; break ;
		case 0x1: Opcode8XY1(opcode) ; break ;
		case 0x2: Opcode8XY2(opcode) ; break ;
		case 0x3: Opcode8XY3(opcode) ; break ;
		case 0x4: Opcode8XY4(opcode) ; break ;
		case 0x5: Opcode8XY5(opcode) ; break ;
		case 0x6: Opcode8XY6(opcode) ; break ;
		case 0x7: Opcode8XY7(opcode) ; break ;
		case 0xE: Opcode8XYE(opcode) ; break ;
		default: break ;
	}
}

// set vx to vy
void ChipCPU::Opcode8XY0(WORD opcode){
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	m_Registers[regx] = m_Registers[regy] ;
}

// VX = VX | VY
void ChipCPU::Opcode8XY1(WORD opcode){
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	m_Registers[regx] = m_Registers[regx] | m_Registers[regy] ;
}

// VX = VX & VY
void ChipCPU::Opcode8XY2(WORD opcode){
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	m_Registers[regx] = m_Registers[regx] & m_Registers[regy] ;
}

// VX = VX xor VY
void ChipCPU::Opcode8XY3(WORD opcode){
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	m_Registers[regx] = m_Registers[regx] ^ m_Registers[regy] ;
}

// add vy to vx. set carry to 1 if overflow otherwise 0
void ChipCPU::Opcode8XY4(WORD opcode){
	m_Registers[0xF] = 0 ;
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	int value = m_Registers[regx] + m_Registers[regy] ;

	if (value > 255)
		m_Registers[0xF] = 1 ;

	m_Registers[regx] = m_Registers[regx] + m_Registers[regy] ;
}

// sub vy from vx. set carry to 1 if no borrow otherwise 0
void ChipCPU::Opcode8XY5(WORD opcode){
	m_Registers[0xF] = 1 ;

	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	if (m_Registers[regx] < m_Registers[regy])
		m_Registers[0xF] = 0 ;

	m_Registers[regx] = m_Registers[regx] - m_Registers[regy] ;
}

// Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
void ChipCPU::Opcode8XY6(WORD opcode){
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	m_Registers[0xF] = m_Registers[regx] & 0x1 ;
	m_Registers[regx] >>= 1 ;
}

// Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
void ChipCPU::Opcode8XY7(WORD opcode){
	m_Registers[0xF] = 1 ;

	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	if (m_Registers[regy] < m_Registers[regx])
		m_Registers[0xF] = 0 ;

	m_Registers[regx] = m_Registers[regy] - m_Registers[regx] ;
}

// Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
void ChipCPU::Opcode8XYE(WORD opcode){
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	m_Registers[0xF] = m_Registers[regx] >> 7 ;
	m_Registers[regx] <<= 1 ;

}

// skip next instruction if VX != VY
void ChipCPU::Opcode9(WORD opcode){
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	if (m_Registers[regx] != m_Registers[regy])
		m_ProgramCounter += 2 ;
}

// set I to nnn
void ChipCPU::OpcodeA(WORD opcode){
	m_AddressI = opcode & 0x0FFF ;
}

// jump to address NNN + V0
void ChipCPU::OpcodeB(WORD opcode){
	int nnn = opcode & 0x0FFF ;
	m_ProgramCounter = m_Registers[0] + nnn ;
}

// set vx to rand + NN
void ChipCPU::OpcodeC(WORD opcode){
	int nn = opcode & 0x00FF ;
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	m_Registers[regx] = rand() & nn ;
}

// Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
// As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
// and to 0 if that doesn't happen
void ChipCPU::OpcodeD(WORD opcode)
{
	const int SCALE = 10 ;
	int regx = opcode & 0x0F00 ;
	regx = regx >> 8 ;
	int regy = opcode & 0x00F0 ;
	regy = regy >> 4 ;

	int coordx = m_Registers[regx] * SCALE;
	int coordy = m_Registers[regy] * SCALE ;
	int height = opcode & 0x000F ;

	m_Registers[0xf] = 0 ;

	for (int yline = 0; yline < height; yline++)
	{
		// this is the data of the sprite stored at m_GameMemory[m_AddressI]
		// the data is stored as a line of bytes so each line is indexed by m_AddressI + yline
		BYTE data = (m_GameMemory[m_AddressI+yline]);

		// for each of the 8 pixels in the line
		int xpixel = 0 ;
		int xpixelinv = 7 ;
		for(xpixel = 0; xpixel < 8; xpixel++, xpixelinv--)
		{
			
			// is ths pixel set to 1? If so then the code needs to toggle its state
			int mask = 1 << xpixelinv ;
			if (data & mask)
			{
				int x = (xpixel*SCALE) + coordx ;
				int y = coordy + (yline*SCALE) ;

				int colour = 0 ;

				// a collision has been detected
				if (m_ScreenData[y][x][0] == 0)
				{
					colour = 255 ;
					m_Registers[15]=1;
				}

				// colour the pixel
				for (int i = 0; i < SCALE; i++)
				{
					for (int j = 0; j < SCALE; j++)
					{
						m_ScreenData[y+i][x+j][0] = colour ;
						m_ScreenData[y+i][x+j][1] = colour ;
						m_ScreenData[y+i][x+j][2] = colour ;
					}
				}

			}
		}
	}
}

void ChipCPU::OpcodeE(WORD opcode)
{
	switch(opcode & 0xF)
	{
		case 0xE: OpcodeEX9E(opcode) ; break ;
		case 0x1: OpcodeEXA1(opcode) ; break ;
		default:break ;
	}
}

void ChipCPU::OpcodeF(WORD opcode)
{
	switch(opcode & 0xFF)
	{
		case 0x07: OpcodeFX07(opcode) ; break ;
		case 0x0A: OpcodeFX0A(opcode) ; break ;
		case 0x15: OpcodeFX15(opcode) ; break ;
		case 0x18: OpcodeFX18(opcode) ; break ;
		case 0x1E: OpcodeFX1E(opcode) ; break ;
		case 0x29: OpcodeFX29(opcode) ; break ;
		case 0x33: OpcodeFX33(opcode) ; break ;
		case 0x55: OpcodeFX55(opcode) ; break ;
		case 0x65: OpcodeFX65(opcode) ; break ;
		default: break ;
	}
}
