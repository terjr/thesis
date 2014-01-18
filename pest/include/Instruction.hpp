class Instruction
{
    public:
        static enum InstrType {
            IntAlu,
            IntMult,
            MemRead,
            MemWrite,
            SimdFloatMisc
        };

        Instruction(std::string instruction);
        virtual ~Instruction();

        static InstrType instrTypeFromString(string instrType);

    private:
        InstrType instrType;

};
