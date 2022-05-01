# ===============================================================
# Purpose:	Makefile for assignment 6
# Author:	Connor Funk
# Date:		Apr. 19, 2022
# ===============================================================
# Variables

CC = g++

SRCS = echo_s.cpp echo_c.cpp log_s.cpp
OBJS = ${SRCS:.cpp=.o}
EXES = ${SRCS:.cpp=}

# ===============================================================
# Target Specific Variables

$(EXES): OBJ = $(addsuffix .o, $@)

$(OBJS): SRC = ${@:.o=.cpp}

# ===============================================================
# Targets

all:
	make $(EXES)

run: $(EXES)
	./log_s &
	./echo_s 8080 7070 6060 &

kill:
	killall -o 1s ./log_s
	killall -o 1s ./echo_s

$(EXES): $(OBJ)
	$(CC) $(OBJ) -o $@

$(OBJS): $(SRC)
	$(CC) -c $(SRC)

clean:
	rm -f $(OBJS) $(EXES)

# ===============================================================
# Dependencies

echo_c: echo_c.o
echo_c.o: echo_c.cpp

echo_s: echo_s.o
echo_s.o: echo_s.cpp

log_s: log_s.o
log_s.o: log_s.cpp

