#include "Buffer.hpp"
#include "Error.hpp"

#include <termios.h>
#include <unistd.h>

using namespace Kedit;

constexpr Bool alphabetic(Sym input) {
	return (input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z');
}

constexpr Bool numeric(Sym input) {
	return (input >= '0' && input <= '9');
}

termios ogtios_G;

Void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &ogtios_G);
}

Void enterRawMode() {
	tcgetattr(STDIN_FILENO, &ogtios_G);
	termios tios = ogtios_G;
	tios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	tios.c_oflag &= ~(OPOST);
	tios.c_cflag |= (CS8);
	tios.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	tios.c_cc[VMIN] = 1;
	tios.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tios);
}

Void test0() {
	Buffer buffer("/home/k/Projects/Kedit/Tests/Test3");
	Sym input;
	while (true) {
		std::cout << "\n>  ";
		std::cin >> input;
		switch (input) {
		case '/': buffer.cursor().erase(); break;
		default: buffer.cursor().write(input); break;
		}
		buffer.print();
		std::cout << std::endl;
	}

}

Void test1() {
	Buffer buffer("/home/k/Projects/Kedit/Tests/Test1");
	buffer.print();
	puts("\n==========================");
	buffer.cursor().write('Z');
	buffer.cursor().write('Z');
	buffer.cursor().erase();
	buffer.print();
}

Void test2() {
	enterRawMode();
	Buffer buffer("/home/k/Projects/Kedit/Tests/Test1");
	Bit input;
	std::cout << "\e[H\e[J";
	Bool running = true;
	while (running) {
		input = 0;
		if (read(STDIN_FILENO, &input, 1) == -1 && errno != EAGAIN)
			break;
		std::cout << "\e[H\e[K";
		switch (input) {
		case 127: buffer.cursor().erase(); break;
		case 'q': running = false; break;
		default:
			if (std::iscntrl(input))
				input = ' ';
			buffer.cursor().write(input);
		}
		buffer.print();
		puts("\r");
	}
	disableRawMode();
}

Void test3() {
	Buffer buffer("/home/k/Projects/Kedit/Tests/Test3");
	for (Int i = 0; i < 5; ++i)
		buffer.cursor().erase();
	buffer.cursor().write();
}

Int32 main() {
	test0();
	return 0;
}
