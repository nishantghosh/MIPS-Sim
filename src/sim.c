/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   ECE 447                                                   */
/*   Carnegie Mellon University                                */
/*                                                             */
/***************************************************************/

/* Lab 1.a Spring 2016
 * Complete the execute() function below by adding cases
 * for the missing opcodes and subocodes (see mips.h). 
 *
 * **NOTE*** You are only allowed to make modifications in the 
 * three specifically designated regions.
 */

#include <stdio.h>
#include "shell.h"
#include "mips.h"

uint32_t dcd_op;     /* decoded opcode */
uint32_t dcd_rs;     /* decoded rs operand */
uint32_t dcd_rt;     /* decoded rt operand */
uint32_t dcd_rd;     /* decoded rd operand */
uint32_t dcd_shamt;  /* decoded shift amount */
uint32_t dcd_funct;  /* decoded function */
uint32_t dcd_imm;    /* decoded immediate value */
uint32_t dcd_target; /* decoded target address */
int      dcd_se_imm; /* decoded sign-extended immediate value */
uint32_t inst;       /* machine instruction */
int64_t prod;
int temp;
uint32_t sign_extend_b2w(uint8_t c)
{
  return (c & 0x80) ? (c | 0xffffff80) : c;
}

uint32_t sign_extend_h2w(uint16_t c)
{
  return (c & 0x8000) ? (c | 0xffff8000) : c;
}

/****************************************************************/
/*** you may add your own auxiliary functions below this line ***/

uint32_t zero_extend_b2w(uint8_t c)
{
    return ((uint32_t) c);
}

uint32_t zero_extend_h2w(uint16_t c)
{
    return ((uint32_t) c);
}

int sign_extend_18b()
{
  return (dcd_se_imm << 2);
}

/*** you may add your own auxiliary functions above this line ***/
/****************************************************************/

void process_instruction()
{
  /*  function: process_instruction
   *  
   *    Process one instruction at a time  
   */

  /* fetch the 4 bytes of the current instruction */
  inst = mem_read_32(CURRENT_STATE.PC);

  /* decode an instruction into fields */
  dcd_op     = (inst >> 26) & 0x3F;
  dcd_rs     = (inst >> 21) & 0x1F;
  dcd_rt     = (inst >> 16) & 0x1F;
  dcd_rd     = (inst >> 11) & 0x1F;
  dcd_shamt  = (inst >> 6) & 0x1F;
  dcd_funct  = (inst >> 0) & 0x3F;
  dcd_imm    = (inst >> 0) & 0xFFFF;
  dcd_se_imm = sign_extend_h2w(dcd_imm);
  dcd_target = (inst >> 0) & ((1UL << 26) - 1);

  /* update architectural state according to instruction */
  switch (dcd_op) {
  case OP_SPECIAL: 
    { 
      switch (dcd_funct) {
      case SUBOP_ADD: 
      case SUBOP_ADDU: 
	if (dcd_rd!=0)
	  NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rs] + CURRENT_STATE.REGS[dcd_rt];
	NEXT_STATE.PC = CURRENT_STATE.PC + 4;
	break;
      case SUBOP_SYSCALL:
	if (CURRENT_STATE.REGS[2] == 10)
	  RUN_BIT = 0;
	NEXT_STATE.PC = CURRENT_STATE.PC + 4;
	break;
/*************************************************************/
/*** specify the remaining dcd_funct cases below this line ***/
      //AND Operation
      case SUBOP_AND:
       if(dcd_rd!=0)
         NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rs] & CURRENT_STATE.REGS[dcd_rt];
       NEXT_STATE.PC = CURRENT_STATE.PC + 4;
      break;
      
      //DIV Operation
     case SUBOP_DIV:
       if(dcd_rt != 0){
        NEXT_STATE.HI = (int)CURRENT_STATE.REGS[dcd_rs] % (int)CURRENT_STATE.REGS[dcd_rt];
        NEXT_STATE.LO = (int)CURRENT_STATE.REGS[dcd_rs] / (int)CURRENT_STATE.REGS[dcd_rt];
      }
      else
        printf("Division by zero. Value unpredictable");
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
      break;
      
      //Unsigned DIV Operation
     case SUBOP_DIVU:
       if(dcd_rt != 0){
        NEXT_STATE.HI = CURRENT_STATE.REGS[dcd_rs] % CURRENT_STATE.REGS[dcd_rt];
        NEXT_STATE.LO = CURRENT_STATE.REGS[dcd_rs] / CURRENT_STATE.REGS[dcd_rt];
      }
      else
        printf("Division by zero. Value unpredictable");
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
      break;
      
      //Jump and Link Register
     case SUBOP_JALR:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.PC + 8;
      if(!(CURRENT_STATE.REGS[dcd_rs] & 0x03))
        NEXT_STATE.PC = CURRENT_STATE.REGS[dcd_rs];
      else
        printf(" Address not word aligned ");
     break;
     
     //Jump Register
     case SUBOP_JR:
      if(!(CURRENT_STATE.REGS[dcd_rs] & 0x03))
        NEXT_STATE.PC = CURRENT_STATE.REGS[dcd_rs];
      else{
        printf(" Address not word aligned ");
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
      }
     break;
     
     //Move from High Register
     case SUBOP_MFHI:
      if(dcd_rd != 0)
       NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.HI;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //Move from Low register
     case SUBOP_MFLO:
      if(dcd_rd != 0)
       NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.LO;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //Move to High register
     case SUBOP_MTHI:
      if(dcd_rs != 0)
        NEXT_STATE.HI = CURRENT_STATE.REGS[dcd_rs];
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //Move to Low register
     case SUBOP_MTLO:
      if(dcd_rs != 0)
        NEXT_STATE.LO = CURRENT_STATE.REGS[dcd_rs];
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //Multiply opration
     case SUBOP_MULT:
      prod = ((int)CURRENT_STATE.REGS[dcd_rs] * (int)CURRENT_STATE.REGS[dcd_rt]);
      NEXT_STATE.HI = (prod >> 31) & 0xFFFFFFFF;
      NEXT_STATE.LO = (prod) & 0xFFFFFFFF;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //Unsigned multiply operation
     case SUBOP_MULTU:
      NEXT_STATE.HI = ((CURRENT_STATE.REGS[dcd_rs] * CURRENT_STATE.REGS[dcd_rt]) >> 31);
      NEXT_STATE.LO = (CURRENT_STATE.REGS[dcd_rs] * CURRENT_STATE.REGS[dcd_rt]) & 0xFFFFFFFF;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //NOR operation
     case SUBOP_NOR:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = ~(CURRENT_STATE.REGS[dcd_rs] | CURRENT_STATE.REGS[dcd_rt]);
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //OR operation
     case SUBOP_OR:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = (CURRENT_STATE.REGS[dcd_rs] | CURRENT_STATE.REGS[dcd_rt]);
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //SHift Left logical
     case SUBOP_SLL:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] << dcd_shamt;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //Shift Word Left Logical Variable
     case SUBOP_SLLV:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] << (CURRENT_STATE.REGS[dcd_rs] & 0x1F);
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //Set on Less than
     case SUBOP_SLT:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = ((int)CURRENT_STATE.REGS[dcd_rs] < (int)CURRENT_STATE.REGS[dcd_rt]) ? 1 : 0;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //Set on less than unsigned
     case SUBOP_SLTU:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = (CURRENT_STATE.REGS[dcd_rs] < CURRENT_STATE.REGS[dcd_rt]) ? 1 : 0;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //Shift right Arithmetic
     case SUBOP_SRA:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = (int)CURRENT_STATE.REGS[dcd_rt] >> dcd_shamt;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //Shift Word Right Arithmetic Variable
     case SUBOP_SRAV:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = (int)CURRENT_STATE.REGS[dcd_rt] >> (CURRENT_STATE.REGS[dcd_rs] & 0x1F);
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //Shift Right Logical
     case SUBOP_SRL:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] >> dcd_shamt;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //Shift Right Word logical Variable
     case SUBOP_SRLV:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] >> (CURRENT_STATE.REGS[dcd_rs] & 0x1F);
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //Subtract
     case SUBOP_SUB:
     case SUBOP_SUBU:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rs] - CURRENT_STATE.REGS[dcd_rt];
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     
     //XOR Operation
     case SUBOP_XOR:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] ^ CURRENT_STATE.REGS[dcd_rs];
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
/*** specify the remaining dcd_funct cases above this line ***/
/*************************************************************/
      default:
	printf("Encountered unimplemented subopcode (0x%x). Ending simulation...\n\n", dcd_funct);
	RUN_BIT = 0;
      } /* switch (dcd_funct) */
    } /* case OP_SPECIAL */
    break;
  case OP_ADDI:
  case OP_ADDIU:
    if (dcd_rt!=0)
      NEXT_STATE.REGS[dcd_rt] = CURRENT_STATE.REGS[dcd_rs] + dcd_se_imm;
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
/**********************************************************/
/*** specify the remaining dcd_op cases below this line ***/
  case OP_BRSPEC: /* special branches */
    {
      switch (dcd_rt) {
      //Branch on Less than Zero
      case BROP_BLTZ:  
       NEXT_STATE.PC = CURRENT_STATE.PC + 4;
       if((int) CURRENT_STATE.REGS[dcd_rs]<0)
         NEXT_STATE.PC = NEXT_STATE.PC + sign_extend_18b();
       break;
      
      //Branch if Greater than or equal to Zero
      case BROP_BGEZ:
       NEXT_STATE.PC = CURRENT_STATE.PC + 4;
       if((int) CURRENT_STATE.REGS[dcd_rs]>=0)
         NEXT_STATE.PC = NEXT_STATE.PC + sign_extend_18b();
      break;
      
      //Branch on Less Than Zero and Link
      case BROP_BLTZAL:
       NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
       if((int) CURRENT_STATE.REGS[dcd_rs]< 0)
         NEXT_STATE.PC = CURRENT_STATE.PC + 4 + sign_extend_18b();
       else 
         NEXT_STATE.PC = CURRENT_STATE.PC + 4;
      break;
      
      //Branch on Greater Than or Equal to Zero and Link
      case BROP_BGEZAL:
       NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
       if((int)CURRENT_STATE.REGS[dcd_rs]>= 0)
         NEXT_STATE.PC = CURRENT_STATE.PC + 4 + sign_extend_18b();
       else
         NEXT_STATE.PC = CURRENT_STATE.PC + 4;
      break;
      }
    }/* special branches */
    break;
    
  //Set on Less Than Immediate
  case OP_SLTI:
      NEXT_STATE.REGS[dcd_rt] = (((int) CURRENT_STATE.REGS[dcd_rs] < (int) dcd_se_imm) ? 1 : 0);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
  
  //Set on Less Than Immediate Unsigned
  case OP_SLTIU:
       NEXT_STATE.REGS[dcd_rt] = ((CURRENT_STATE.REGS[dcd_rs] < dcd_se_imm) ? 1 : 0);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
    
  //AND with immidiate
  case OP_ANDI:
       NEXT_STATE.REGS[dcd_rt] = (CURRENT_STATE.REGS[dcd_rs] & zero_extend_h2w(dcd_imm));
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
  
  //OR with immidiate
  case OP_ORI:
       NEXT_STATE.REGS[dcd_rt] = (CURRENT_STATE.REGS[dcd_rs] | zero_extend_h2w(dcd_imm));
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
    
  //XOR with immidiate
  case OP_XORI:
       NEXT_STATE.REGS[dcd_rt] = (CURRENT_STATE.REGS[dcd_rs] ^ zero_extend_h2w(dcd_imm));
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
    
  //JUMP
  case OP_J:
    NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000) | (dcd_target << 2);
    break;
    
  //Jump and Link
  case OP_JAL:
        NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
    NEXT_STATE.PC = (CURRENT_STATE.PC & 0xF0000000) | (dcd_target << 2);
    break;
    
  //Branch if Equal
  case OP_BEQ:
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    if (CURRENT_STATE.REGS[dcd_rs]==CURRENT_STATE.REGS[dcd_rt])
       NEXT_STATE.PC = NEXT_STATE.PC + sign_extend_18b();
    break;
  
  //Branch if not equal
  case OP_BNE:
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    if (CURRENT_STATE.REGS[dcd_rs]!=CURRENT_STATE.REGS[dcd_rt])
       NEXT_STATE.PC = NEXT_STATE.PC + sign_extend_18b();
    break;
    
  //Branch if less than equal to Zero
  case OP_BLEZ:
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    if (CURRENT_STATE.REGS[dcd_rs]<=0)
       NEXT_STATE.PC = NEXT_STATE.PC + sign_extend_18b();
    break;
    
  //branch if Greater than Zero
  case OP_BGTZ:
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    if (CURRENT_STATE.REGS[dcd_rs]>0)
       NEXT_STATE.PC = NEXT_STATE.PC + sign_extend_18b();
    break;
    
  //Load Upper Immediate
  case OP_LUI:
    if (dcd_rs==0)
      NEXT_STATE.REGS[dcd_rt] = (dcd_imm << 16);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
  
  //Load Byte
  case OP_LB:
      NEXT_STATE.REGS[dcd_rt] = sign_extend_b2w((mem_read_32(CURRENT_STATE.REGS[dcd_rs] + dcd_se_imm)) & 0xFF);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
    
  //Load Halfword
  case OP_LH:
      NEXT_STATE.REGS[dcd_rt] = sign_extend_h2w((mem_read_32(CURRENT_STATE.REGS[dcd_rs] + dcd_se_imm)) & 0xFFFF);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
    
  //Load Word
  case OP_LW:
      NEXT_STATE.REGS[dcd_rt] = mem_read_32((CURRENT_STATE.REGS[dcd_rs] + dcd_se_imm));
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
  
  //Load Byte Unsigned
  case OP_LBU:
      NEXT_STATE.REGS[dcd_rt] = zero_extend_b2w((mem_read_32(CURRENT_STATE.REGS[dcd_rs] + dcd_se_imm)) & 0xFF);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
    
  //Load Halfword Unsigned
  case OP_LHU:
      NEXT_STATE.REGS[dcd_rt] = zero_extend_h2w((mem_read_32(CURRENT_STATE.REGS[dcd_rs] + dcd_se_imm)) & 0xFFFF);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
  
  //Store Byte
  case OP_SB:
      mem_write_32((CURRENT_STATE.REGS[dcd_rs] + dcd_se_imm), (int) ((CURRENT_STATE.REGS[dcd_rt]) & 0xFF));
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
  
  //Store Halfword
  case OP_SH:
      mem_write_32((CURRENT_STATE.REGS[dcd_rs] + dcd_se_imm), (int) ((CURRENT_STATE.REGS[dcd_rt]) & 0xFFFF));
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
  
  //Store Word
  case OP_SW:
      mem_write_32((CURRENT_STATE.REGS[dcd_rs] + dcd_se_imm), (int) ((CURRENT_STATE.REGS[dcd_rt])));
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;

/*** specify the remaining dcd_op cases above this line ***/
/*********************************************************/
  default:
    printf("Encountered unimplemented opcode (0x%x). Ending simulation...\n\n", dcd_op);
    RUN_BIT = 0;
  } /* switch (dcd_op) */
}
