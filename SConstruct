base = Environment()
base.MergeFlags("-Wall -Wextra ")
base["CC"]           = "gcc -m32"
base["CCCOMSTR"]     = "   CC ${TARGET}"
base["ARCOMSTR"]     = "   AR ${TARGET}"
base["LINKCOMSTR"]   = "   LD ${TARGET}"
base["RANLIBCOMSTR"] = "   RL ${TARGET}"

cobject_env = base.Clone();
cobject_env.MergeFlags("-Iinclude")
cobject_env.Library("cobject", Glob("src/cobject/*.c"))

main_env = base.Clone()
main_env.MergeFlags("-Iinclude/ -lcobject -L.")
main_env.Program("main", Split("src/main.c"))
 
cmockery_env = base.Clone()
cmockery_env.MergeFlags("-Itest -D_GNU_SOURCE")
cmockery_env.Library("cmockery", Split("test/cmockery.c"))

test_env = base.Clone()
test_env.MergeFlags("-Iinclude -Itest -lcobject -lcmockery -L.")
test_env.Program("test/list_test", Split("test/cobject/list_test.c"))


 