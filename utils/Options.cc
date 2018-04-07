/**************************************************************************************[Options.cc]

**************************************************************************************************/

#include "../mtl/Sort.h"
#include "../utils/Options.h"
#include "../utils/ParseUtils.h"

using namespace Minisat;

void Minisat::parseOptions(int& argc, char** argv, bool strict)
{
    int i, j;
    //分割每个控制台输入的参数i
    for (i = j = 1; i < argc; i++)
    {
        const char* str = argv[i];
        //若参数是--help则打印帮助信息
        if (match(str, "--") && match(str, Option::getHelpPrefixString()) && match(str, "help"))
        {
            if (*str == '\0')
                printUsageAndExit(argc, argv);
            else if (match(str, "-verb"))
                printUsageAndExit(argc, argv, true);
        }
        else
        {
            bool parsed_ok = false;
            //用参数填写到对应数据类型的Option
            for (int k = 0; !parsed_ok && k < Option::getOptionList().size(); k++)
            {
                parsed_ok = Option::getOptionList()[k]->parse(argv[i]);
                // fprintf(stderr, "checking %d: %s against flag <%s> (%s)\n", i, argv[i], Option::getOptionList()[k]->name, parsed_ok ? "ok" : "skip");
            }
            //参数i的格式错误
            if (!parsed_ok)
                if (strict && match(argv[i], "-"))
                    fprintf(stderr, "ERROR! Unknown flag \"%s\". Use '--%shelp' for help.\n", argv[i], Option::getHelpPrefixString()), exit(1);
                else
                    argv[j++] = argv[i];
        }
    }

    argc -= (i - j);//argc用来决定参数输入错误时的报错
}


void Minisat::setUsageHelp      (const char* str){ Option::getUsageString() = str; }
void Minisat::setHelpPrefixStr  (const char* str){ Option::getHelpPrefixString() = str; }
void Minisat::printUsageAndExit (int argc, char** argv, bool verbose)
{
    const char* usage = Option::getUsageString();
    if (usage != NULL)
        fprintf(stderr, usage, argv[0]);

    sort(Option::getOptionList(), Option::OptionLt());

    const char* prev_cat  = NULL;
    const char* prev_type = NULL;

    for (int i = 0; i < Option::getOptionList().size(); i++){
        const char* cat  = Option::getOptionList()[i]->category;
        const char* type = Option::getOptionList()[i]->type_name;

        if (cat != prev_cat)
            fprintf(stderr, "\n%s OPTIONS:\n\n", cat);
        else if (type != prev_type)
            fprintf(stderr, "\n");

        Option::getOptionList()[i]->help(verbose);

        prev_cat  = Option::getOptionList()[i]->category;
        prev_type = Option::getOptionList()[i]->type_name;
    }

    fprintf(stderr, "\nHELP OPTIONS:\n\n");
    fprintf(stderr, "  --%shelp        Print help message.\n", Option::getHelpPrefixString());
    fprintf(stderr, "  --%shelp-verb   Print verbose help message.\n", Option::getHelpPrefixString());
    fprintf(stderr, "\n");
    exit(0);
}

