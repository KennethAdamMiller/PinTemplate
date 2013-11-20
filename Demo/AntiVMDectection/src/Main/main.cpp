
#include <iostream>
#include <fstream>
#include "pin.H"

using namespace std;

KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool",
	"o", "anti_vm_detection.txt", "specify output file name");

ofstream outputFile;

VOID BeforeIn(const CONTEXT * ctxt, THREADID tid, ADDRINT *pGax, ADDRINT *pGdx)
{
	ADDRINT exceptAddr = PIN_GetContextReg(ctxt, REG_INST_PTR);

	outputFile << hex << __FUNCTION__ << "\t" << exceptAddr <<endl
		<< "EAX : " << *pGax << endl
		<< "EDX : " << *pGdx << endl;

	//
	// Raise an exception before executing in.
	// 
	
	EXCEPTION_INFO exceptInfo;
	PIN_InitExceptionInfo(&exceptInfo, EXCEPTCODE_PRIVILEGED_INS, exceptAddr);
	PIN_RaiseException(ctxt, tid, &exceptInfo);
}

VOID Trace(TRACE trace, VOID *v)
{
	for (auto bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl)) {

		for (auto ins = BBL_InsHead(bbl); INS_Valid(ins); ins = INS_Next(ins)) {

			if (INS_Opcode(ins) == XED_ICLASS_IN) {

				auto operand_0 = INS_OperandReg(ins, 0);
				auto operand_1 = INS_OperandReg(ins, 1);

				if (operand_0 != REG_GAX && 
					operand_1 != REG_DX)
					continue;

				//
				// check if it's in eax, dx.
				// 

				outputFile << hex << "found IN at : " << INS_Address(ins) 
					<< "\t" << INS_Disassemble(ins) << endl;
				
				//
				// insert a call before in eax, dx instruction.
				// 

				INS_InsertCall(ins,
					IPOINT_BEFORE,
					AFUNPTR(BeforeIn),
					IARG_CONST_CONTEXT,
					IARG_THREAD_ID,
					IARG_REG_REFERENCE, REG_GAX,
					IARG_REG_REFERENCE, REG_GDX,
					IARG_CONTEXT,
					IARG_END);
			}
		}
   }
}


VOID Fini(INT32 code, VOID *v)
{
	outputFile.close();
}

INT32 Usage()
{
    return -1;
}

int main(int argc, char * argv[])
{
    if (PIN_Init(argc, argv)) 
		return Usage();

	outputFile.open(KnobOutputFile.Value());

    TRACE_AddInstrumentFunction(Trace, 0);

    PIN_AddFiniFunction(Fini, 0);
    
    PIN_StartProgram();
    
    return 0;
}
