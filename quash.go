package main

import (
	"bufio"
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

func getStdin(c chan string) {
	reader := bufio.NewReader(os.Stdin)
	input, err := reader.ReadString('\n')

	if err != nil {
		c <- input
	} else {
		fmt.Fprintln(os.Stderr, "Error reading from stdin:", err)
		os.Exit(0)
	}
}

func main() {
	systemChannel := make(chan os.Signal, 10)
	stdinChannel := make(chan string)
	signal.Notify(systemChannel)

	go getStdin(stdinChannel)

	for {
		fmt.Println("[Quash: ] $")

		select {
		case signal := <-systemChannel:
			handleSystemSignal(signal)
		case input := <-stdinChannel:
			job, err := parse(input)
			if err != nil {
				execute(job)
			} else {
				fmt.Fprintln(os.Stderr, "Error parsing:", err)
			}
		}
	}

}

func handleSystemSignal(s os.Signal) {
	fmt.Println("Handled signal:", s)
}

func parse(input string) (job, error) {
	return job{}, fmt.Errorf("error in parse")
}

func execute(j job) error {
	return fmt.Errorf("Error in execute")
}

func helpBuiltin() int {
	fmt.Println("help")
	return 0
}

func init() {
	jobs = make([]job, 0)
}
