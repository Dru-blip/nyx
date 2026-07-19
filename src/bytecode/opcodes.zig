pub const Opcode = enum(u8) {
    load_imm_int,
    ret,

    pub fn to(self: Opcode) u8 {
        return switch (self) {
            .load_imm_int => 0,
            .ret => 1,
            else => 0,
        };
    }
};
