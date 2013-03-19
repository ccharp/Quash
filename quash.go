package main

import (
	"fmt"
	"os"
	"os/signal"
)

// Globals

var builtins = map[string]builtinFunction{
	"help": helpBuiltin,
}

var jobs []job

// Types

type builtinFunction func() int

type baseCommand struct {
	input     string
	arguments []string
}

type process struct {
	baseCommand
	binaryPath string
}

type builtinCommand struct {
	baseCommand
	fun builtinFunction
}

type job struct {
	commands     []command
	backgrounded bool
	jobId        int
}

type command interface {
	Run() int
}

// Methods

func (p process) Run() int {
	fmt.Println("run", p.input, p.arguments)
	return 0
}

func (bi builtinCommand) Run() int {
	fmt.Println("run", bi.input, bi.arguments)
	return 0
}

func main() {
	signalChannel := make(chan os.Signal, 10)
	signal.Notify(signalChannel)

	fmt.Println("quash")
	for {
		s := <-signalChannel
		fmt.Println("got signal", s)
	}

}

func parse(input string) job {
	return job{}
}

func helpBuiltin() int {
	fmt.Println("help")
	return 0
}

func init() {
	jobs = make([]job, 0)
}
