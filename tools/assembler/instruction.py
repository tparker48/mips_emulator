import struct

from tools.assembler.registers import Registers


class Instruction:
    def __init__(self, op_code=None, rd=None, rs=None, rt=None, shamt=None, funct=None, immediate=None, address=None):
        self.op_code = int(op_code) if op_code else 0
        self.funct = int(funct, 0) if funct else 0
        self.shamt = int(shamt, 0) if shamt else 0
        self.immediate = int(immediate, 0) if immediate else 0
        self.address = int(address, 0) if address else 0
        self.rd = self.reg_map(rd) if rd else 0
        self.rs = self.reg_map(rs) if rs else 0
        self.rt = self.reg_map(rt) if rt else 0
        
    def bytes(self) -> bytes:
        if (self.op_code in [0x00, 0x10]):
            return self.__r_type_bytes()
        elif (self.op_code in [0x02, 0x03]):
            return self.__j_type_bytes()
        else:
            return self.__i_type_bytes()

    def __r_type_bytes(self) -> bytes:
        raw = self.op_code
        raw = (raw << 5) | self.rs
        raw = (raw << 5) | self.rt
        raw = (raw << 5) | self.rd
        raw = (raw << 5) | self.shamt
        raw = (raw << 6) | self.funct
        return struct.pack('>I', raw)
    
    def __i_type_bytes(self) -> bytes:
        raw = self.op_code
        raw = (raw << 5)  | self.rs
        raw = (raw << 5)  | self.rt
        raw = (raw << 16) | self.immediate
        return struct.pack('>I', raw)
    
    def __j_type_bytes(self) -> bytes:
        raw = self.op_code
        raw = (raw << 26) | self.address
        return struct.pack('>I', raw)
    
    def reg_map(self, reg: str) -> int:
        return Registers[reg.strip('$')].value