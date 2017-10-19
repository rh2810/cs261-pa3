#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <check.h>

#include "../p3-disas.h"

#define testsuite_memsize 24
uint8_t array[testsuite_memsize];
memory_t testsuite_memory = array;

/* check HALT handling */
START_TEST (D_fetch_halt)
{
    y86_t cpu;
    uint8_t opcode = HALT << 4;
    uint8_t opsize = 1;
    cpu.pc = (y86_register_t) (rand () % (testsuite_memsize - opsize + 1));
    memset (testsuite_memory, 0, testsuite_memsize);
    memset (&cpu, 0, sizeof (cpu));
    testsuite_memory[cpu.pc] = opcode;

    y86_inst_t inst = fetch (cpu, testsuite_memory);
    ck_assert (inst.type   == HALT);
    ck_assert (inst.size   == opsize);
    ck_assert (inst.opcode == opcode);
}
END_TEST

/* check NOP handling */
START_TEST (D_fetch_nop)
{
    y86_t cpu;
    uint8_t opcode = NOP << 4;
    uint8_t opsize = 1;
    memset (testsuite_memory, 0, testsuite_memsize);
    memset (&cpu, 0, sizeof (cpu));
    cpu.pc = (y86_register_t) (rand () % (testsuite_memsize - opsize + 1));
    testsuite_memory[cpu.pc] = opcode;

    y86_inst_t inst = fetch (cpu, testsuite_memory);
    ck_assert (inst.type   == NOP);
    ck_assert (inst.size   == opsize);
    ck_assert (inst.opcode == opcode);
}
END_TEST

/* check RET handling */
START_TEST (D_fetch_ret)
{
    y86_t cpu;
    uint8_t opcode = RET << 4;
    uint8_t opsize = 1;
    memset (testsuite_memory, 0, testsuite_memsize);
    memset (&cpu, 0, sizeof (cpu));
    cpu.pc = (y86_register_t) (rand () % (testsuite_memsize - opsize + 1));
    testsuite_memory[cpu.pc] = opcode;

    y86_inst_t inst = fetch (cpu, testsuite_memory);
    ck_assert (inst.type   == RET);
    ck_assert (inst.size   == opsize);
    ck_assert (inst.opcode == opcode);
}
END_TEST

/* check HALT handling w/ errors */
START_TEST (D_errors_fetch_halt)
{
    y86_t cpu;
    uint8_t opcode;
    memset (&cpu, 0, sizeof (cpu));
    uint8_t i;
    size_t opsize = 1;

    for (i = 1; i < 16; i++)
    {
        opcode = (HALT << 4) | i;   // set low-order bits to non-zero
        memset (testsuite_memory, 0, testsuite_memsize);
        cpu.pc = (y86_register_t) (rand () % (testsuite_memsize - opsize + 1));
        testsuite_memory[cpu.pc] = opcode;
        y86_inst_t inst = fetch (cpu, testsuite_memory);
        ck_assert (inst.type   == INVALID);
    }
}
END_TEST

/* check NOP handling w/ errors */
START_TEST (D_errors_fetch_nop)
{
    y86_t cpu;
    uint8_t opcode;
    memset (&cpu, 0, sizeof (cpu));
    uint8_t i;
    size_t opsize = 1;

    for (i = 1; i < 16; i++)
    {
        opcode = (NOP << 4) | i;   // set low-order bits to non-zero
        memset (testsuite_memory, 0, testsuite_memsize);
        cpu.pc = (y86_register_t) (rand () % (testsuite_memsize - opsize + 1));
        testsuite_memory[cpu.pc] = opcode;
        y86_inst_t inst = fetch (cpu, testsuite_memory);
        ck_assert (inst.type   == INVALID);
    }
}
END_TEST

/* check NOP handling w/ errors */
START_TEST (D_errors_fetch_ret)
{
    y86_t cpu;
    uint8_t opcode;
    uint8_t opsize = 1;
    uint8_t i;
    y86_inst_t inst;
    memset (testsuite_memory, 0, testsuite_memsize);
    memset (&cpu, 0, sizeof (cpu));
    cpu.pc = (y86_register_t) (rand () % (testsuite_memsize - opsize + 1));

    for (i = 1; i < 16; i++)
    {
        opcode = (RET << 4) | i;   // set low-order bits to non-zero
        testsuite_memory[cpu.pc] = opcode;
        inst = fetch (cpu, testsuite_memory);
        ck_assert (inst.type   == INVALID);
    }
}
END_TEST

void public_tests (Suite *s)
{
    TCase *tc_public = tcase_create ("Public");
    tcase_add_test (tc_public, D_fetch_halt);
    tcase_add_test (tc_public, D_fetch_nop);
    tcase_add_test (tc_public, D_fetch_ret);
    tcase_add_test (tc_public, D_errors_fetch_halt);
    tcase_add_test (tc_public, D_errors_fetch_nop);
    tcase_add_test (tc_public, D_errors_fetch_ret);
    suite_add_tcase (s, tc_public);
}

