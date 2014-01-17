class Instruction
{
    public:
        enum InstrType {
            IntAlu,
            IntMult,
            MemRead,
            MemWrite,
            SimdFloatMisc
        };

        Instruction();

    InstrType instrTypeFromString
