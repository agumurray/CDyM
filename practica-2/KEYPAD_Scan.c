char KEYPAD_Scan(char *key){
    char r,c;
    
    PORTB |= 0x0F;

    for (c=0; c<4; c++)
    {
        DDRB &= (0xFF);
        DDRB |= (0x80>>c);

        for (r=0; r<4; 4++)
        {
            if(!(PINB & (0x08>>r)))
            {
                key = r*4+c;
                return 1;
            }
        }
    }

    return 0;
}

int main() {
    return 0;
}