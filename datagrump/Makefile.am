AM_CPPFLAGS = $(CXX11_FLAGS) -I$(srcdir)/../src
AM_CXXFLAGS = $(PICKY_CXXFLAGS)
LDADD = ../src/libsourdough.a -lpthread

common_source = contest_message.hh contest_message.cc \
	controller.hh controller.cc \
	controller-aimd.hh controller-aimd.cc \
	controller-delay.hh controller-delay.cc \
	controller-contest.hh controller-contest.cc


bin_PROGRAMS = sender receiver

sender_SOURCES = $(common_source) sender.cc

receiver_SOURCES = $(common_source) receiver.cc
