/****************************************************************************
  FileName     [ main.cpp ]
  PackageName  [ main ]
  Synopsis     [ Define main() ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2012 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cstdlib>
#include "util.h"
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Global cmd Manager
//----------------------------------------------------------------------
CmdParser* cmdMgr = new CmdParser("adt> ");

extern bool initCommonCmd();
extern bool initAdtCmd();

static void
usage()
{
   cout << "Usage: adtTest.xxxx [ -File < doFile > ]" << endl;
}

static void
myexit()
{
   usage();
   exit(-1);
}

int
main(int argc, char** argv)
{
   myUsage.reset();

   ifstream dof;

   if (argc == 3) {  // -file <doFile>
      if (myStrNCmp("-File", argv[1], 2) == 0) {
         if (!cmdMgr->openDofile(argv[2])) {
            cerr << "Error: cannot open file \"" << argv[2] << "\"!!\n";
            myexit();
         }
      }
      else {
         cerr << "Error: unknown argument \"" << argv[1] << "\"!!\n";
         myexit();
      }
   }
   else if (argc != 1) {
      cerr << "Error: illegal number of argument (" << argc << ")!!\n";
      myexit();
   }

   if (!initCommonCmd() || !initAdtCmd())
      return 1;

   CmdExecStatus status = CMD_EXEC_DONE;
   while (status != CMD_EXEC_QUIT) {  // until "quit" or command error
      status = cmdMgr->execOneCmd();
      cout << endl;  // a blank line between each command
   }

   return 0;
}