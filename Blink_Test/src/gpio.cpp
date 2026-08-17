#include "gpio.hpp"

void UART::init() {
    // Set RXENn and TXENn bits in UCSRnB to enable receiver and transmitter (4, 3 bit)
    // Set UCSZn2 bit to 0 (8-bit communication) (2 bit)
    UCSR0B = 0x18;

    // Set datasize for communication Asynchronous USART
    // Disable parity communication
    // Set stop bit to 1 bit (one time)
    // Set UCSZn0 and UCSZn1 bits to 1 (8-bit communication) (1, 2 bit)
    UCSR0C = 0x06;

    // Set speed of communication - double the USART Transmission speed (1 bit)
    UCSR0A = 0x02;

    // Set baud rate to 9600 (UBRN0 = 207 for U2X0 = 1)
    UBRR0 = 0xCF;
}

unsigned char UART::receive() {
    // Wait for data to be received
    // if flag RXC0 if rising
    // then hardware will clean the flag
    while (!(UCSR0A & (1 << RXC0)));

    // Get and return received data from buffer
    return UDR0; 
}

void UART::send(char ch) {
    // Wait for register flag to be empty
    while (!(UCSR0A & (1 << UDRE0)));

    // Send the char
    UDR0 = ch;
}