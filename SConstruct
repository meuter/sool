Alias("all", ".")
CacheDir("/tmp/")
Decider('MD5-timestamp')
SetOption('max_drift', 1)
SetOption('implicit_cache', 1)
SourceCode(".", None)
SetOption('num_jobs', 3)

def UnitTest(self, target, source):
    self.Program(target, source)
    self.Command(target + ".res", target, self.Action(target + " > " + target.strip() + ".res", "   TEST  " + target))
    
base = Environment()
AddMethod(Environment, UnitTest)
base["CC"]           = "gcc -m32"
base["CCCOMSTR"]     = "   CC    ${TARGET}"
base["SHCCCOMSTR"]   = "   CC    ${TARGET}"
base["SHLINKCOMSTR"] = "   LD    ${TARGET}"
base["ARCOMSTR"]     = "   AR    ${TARGET}"
base["LINKCOMSTR"]   = "   LD    ${TARGET}"
base["RANLIBCOMSTR"] = "   RL    ${TARGET}"

########################################################################################################

root = base.Clone()
root.MergeFlags("-Wall -Wextra -O3")
root.MergeFlags("-ggdb -O0")

sool_env = root.Clone();
sool_env.MergeFlags("-Iinclude")
sool_env.Library("sool", Glob("src/sool/*.c"))

main_env = root.Clone()
main_env.MergeFlags("-Iinclude/ -lsool -L. -lgc -lpthread")
main_env.Program("main", Split("src/main.c"))
 
Default(Split("main libsool.a"))
 
cmockery_env = base.Clone()
cmockery_env.MergeFlags("-Itest -D_GNU_SOURCE -ggdb")
cmockery_env.Library("test/cmockery", Split("test/cmockery.c"))

test_env = root.Clone()
test_env.MergeFlags("-Iinclude -Itest -lsool -lcmockery -Ltest -L. -lgc -lpthread")
for test_source in Glob("test/sool/*_test.c"):
    test_bin = str(test_source).replace(".c","")
    test_env.UnitTest(test_bin, test_source)
