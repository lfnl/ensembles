#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = g++

# define any compile-time flags
CFLAGS = -std=c++11 -Wall -g

# define any directories containing header files other than /usr/include
#

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = -l boost_program_options -lboost_filesystem -lboost_system -l pthread -l z

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:

# define the C source files
P=/home/arthur/Documents/vowpal_wabbit/vowpalwabbit/
SRCS = main.cpp classifier.cpp classifier.h $(P)accumulate.o $(P)active.o $(P)allreduce.o $(P)autolink.o $(P)best_constant.o $(P)bfgs.o $(P)binary.o $(P)boosting.o $(P)bs.o $(P)cache.o $(P)cb_adf.o $(P)cb_algs.o $(P)cb.o $(P)cbify.o $(P)comp_io.o $(P)cost_sensitive.o $(P)csoaa.o $(P)ect.o $(P)example.o $(P)ftrl.o $(P)gd.o $(P)gd_mf.o $(P)global_data.o $(P)hash.o $(P)interact.o $(P)interactions.o $(P)io_buf.o $(P)kernel_svm.o $(P)label_dictionary.o $(P)lda_core.o $(P)learner.o $(P)log_multi.o $(P)loss_functions.o $(P)lrq.o $(P)lrqfa.o $(P)mf.o $(P)multiclass.o $(P)multilabel.o $(P)multilabel_oaa.o $(P)network.o $(P)nn.o $(P)noop.o $(P)oaa.o $(P)parse_args.o $(P)parse_example.o $(P)parse_primitives.o $(P)parser.o $(P)parse_regressor.o $(P)print.o $(P)rand48.o $(P)scorer.o $(P)search.o $(P)search_dep_parser.o $(P)search_entityrelationtask.o $(P)search_graph.o $(P)search_hooktask.o $(P)search_meta.o $(P)search_multiclasstask.o $(P)search_sequencetask.o $(P)sender.o $(P)simple_label.o $(P)stagewise_poly.o $(P)svrg.o $(P)topk.o $(P)unique_sort.o $(P)vw_exception.o

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
OBJS = $(SRCS:.cc|.cpp=.o)

# define the executable file 
MAIN = ensemble

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:$(MAIN)
	@echo  'Ensemble' has been compiled

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
