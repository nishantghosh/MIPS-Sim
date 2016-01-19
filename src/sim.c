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
      case SUBOP_AND:
       if(dcd_rd!=0)
         NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rs] & CURRENT_STATE.REGS[dcd_rt];
       NEXT_STATE.PC = CURRENT_STATE.PC + 4;
      break;
     case SUBOP_DIV:
       if(dcd_rt != 0){
        NEXT_STATE.HI = (int)CURRENT_STATE.REGS[dcd_rs] % (int)CURRENT_STATE.REGS[dcd_rt];
        NEXT_STATE.LO = (int)CURRENT_STATE.REGS[dcd_rs] / (int)CURRENT_STATE.REGS[dcd_rt];
      }
      else
        printf("Division by zero. Value unpredictable");
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
      break;
     case SUBOP_DIVU:
       if(dcd_rt != 0){
        NEXT_STATE.HI = CURRENT_STATE.REGS[dcd_rs] % CURRENT_STATE.REGS[dcd_rt];
        NEXT_STATE.LO = CURRENT_STATE.REGS[dcd_rs] / CURRENT_STATE.REGS[dcd_rt];
      }
      else
        printf("Division by zero. Value unpredictable");
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
      break;
     case SUBOP_JALR:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.PC + 8;
      if(!(CURRENT_STATE.REGS[dcd_rs] & 0x03))
        NEXT_STATE.PC = CURRENT_STATE.REGS[dcd_rs];
      else
        printf(" Address not word aligned ");
     break;
     case SUBOP_JR:
      if(!(CURRENT_STATE.REGS[dcd_rs] & 0x03))
        NEXT_STATE.PC = CURRENT_STATE.REGS[dcd_rs];
      else{
        printf(" Address not word aligned ");
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
      }
     break;
     case SUBOP_MFHI:
      if(dcd_rd != 0)
       NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.HI;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_MFLO:
      if(dcd_rd != 0)
       NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.LO;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_MTHI:
      if(dcd_rs != 0)
        NEXT_STATE.HI = CURRENT_STATE.REGS[dcd_rs];
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_MTLO:
      if(dcd_rs != 0)
        NEXT_STATE.LO = CURRENT_STATE.REGS[dcd_rs];
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_MULT:
      prod = ((int)CURRENT_STATE.REGS[dcd_rs] * (int)CURRENT_STATE.REGS[dcd_rt]);
      NEXT_STATE.HI = (prod >> 31) & 0xFFFFFFFF;
      NEXT_STATE.LO = (prod) & 0xFFFFFFFF;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_MULTU:
      NEXT_STATE.HI = ((CURRENT_STATE.REGS[dcd_rs] * CURRENT_STATE.REGS[dcd_rt]) >> 31);
      NEXT_STATE.LO = (CURRENT_STATE.REGS[dcd_rs] * CURRENT_STATE.REGS[dcd_rt]) & 0xFFFFFFFF;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_NOR:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = ~(CURRENT_STATE.REGS[dcd_rs] | CURRENT_STATE.REGS[dcd_rt]);
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_OR:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = (CURRENT_STATE.REGS[dcd_rs] | CURRENT_STATE.REGS[dcd_rt]);
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_SLL:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] << dcd_shamt;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_SLLV:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] << (CURRENT_STATE.REGS[dcd_rs] & 0x1F);
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_SLT:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = ((int)CURRENT_STATE.REGS[dcd_rs] < (int)CURRENT_STATE.REGS[dcd_rt]) ? 1 : 0;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_SLTU:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = (CURRENT_STATE.REGS[dcd_rs] < CURRENT_STATE.REGS[dcd_rt]) ? 1 : 0;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_SRA:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = (int)CURRENT_STATE.REGS[dcd_rt] >> dcd_shamt;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_SRAV:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = (int)CURRENT_STATE.REGS[dcd_rt] >> (CURRENT_STATE.REGS[dcd_rs] & 0x1F);
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_SRL:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] >> dcd_shamt;
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_SRLV:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rt] >> (CURRENT_STATE.REGS[dcd_rs] & 0x1F);
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
     case SUBOP_SUB:
     case SUBOP_SUBU:
      if(dcd_rd != 0)
        NEXT_STATE.REGS[dcd_rd] = CURRENT_STATE.REGS[dcd_rs] - CURRENT_STATE.REGS[dcd_rt];
      NEXT_STATE.PC = CURRENT_STATE.PC + 4;
     break;
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
          
      case OP_SLTI:
    if (dcd_rt!=0)
      NEXT_STATE.REGS[dcd_rt] = (((int) CURRENT_STATE.REGS[dcd_rs] < dcd_se_imm) ? 1 : 0);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
      case OP_SLTIU:
    if (dcd_rt!=0)
       NEXT_STATE.REGS[dcd_rt] = ((CURRENT_STATE.REGS[dcd_rs] < dcd_se_imm) ? 1 : 0);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
      case OP_ANDI:
    if (dcd_rt!=0)
       NEXT_STATE.REGS[dcd_rt] = (CURRENT_STATE.REGS[dcd_rs] & zero_extend_h2w(dcd_imm));
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
      case OP_ORI:
    if (dcd_rt!=0)
       NEXT_STATE.REGS[dcd_rt] = (CURRENT_STATE.REGS[dcd_rs] | zero_extend_h2w(dcd_imm));
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
      case OP_XORI:
    if (dcd_rt!=0)
       NEXT_STATE.REGS[dcd_rt] = (CURRENT_STATE.REGS[dcd_rs] ^ zero_extend_h2w(dcd_imm));
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
      case OP_J:
          CURRENT_STATE.PC >> 28(dcd_target << 2)
      case OP_JAL:
          (dcd_target << 2)
      case OP_BEQ:
    if (dcd_rs==dcd_rt)
       NEXT_STATE.PC = CURRENT_STATE.PC + (dcd_imm << 2);
    break;
      case OP_BNE:
    if (dcd_rs!=dcd_rt)
       NEXT_STATE.PC = CURRENT_STATE.PC + (dcd_imm << 2);
    break;
      case OP_BLEZ:
    if (dcd_rs<=0)
       NEXT_STATE.PC = CURRENT_STATE.PC + (dcd_imm << 2);
    break;
      case OP_BGTZ:
    if (dcd_rs>0)
       NEXT_STATE.PC = CURRENT_STATE.PC + (dcd_imm << 2);
    break;
      case OP_LUI:
    if (dcd_rt!=0)
      NEXT_STATE.REGS[dcd_rt] = (dcd_imm << 16);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    break;
      case OP_LB:
      case OP_LH:
      case OP_LW:
      case OP_LBU:
      case OP_LHU:
      case OP_SB:
      case OP_SH:
      case OP_SW:

/*** specify the remaining dcd_op cases above this line ***/
/*********************************************************/
  default:
    printf("Encountered unimplemented opcode (0x%x). Ending simulation...\n\n", dcd_op);
    RUN_BIT = 0;
  } /* switch (dcd_op) */
}
