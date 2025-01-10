// 0x6080604052348015600e575f80fd5b5060a58061001b5f395ff3fe6080604052348015600e575f80fd5b50600436106030575f3560e01c8063cdfead2e146034578063e026c017146045575b5f80fd5b6043603f3660046059565b5f55565b005b5f5460405190815260200160405180910390f35b5f602082840312156068575f80fd5b503591905056fea2646970667358221220977f8652cc5392ce6356aa33e0e73ee886e4c73f9a9fd33dbf834e70d0b6b79964736f6c63430008140033


// you have:
// creation code
// runtime
// metadata


// 1. creation code
// free memory pointer - telling that at slot 0x40 there is a free memory at 0x80 slot always points alewys to a free memory slot
PUSH1 0x80          // [0x80]
PUSH1 0x40          // [0x40, 0x80]
MSTORE              // [] - memory 0x40 -> 0x80

// if someone sent value, revert
// returns the value of current call in wei
CALLVALUE           //[msg.value] -> [0]
DUP1                // [msg.value,msg.value]
ISZERO              // [msg.value == 0,msg.value]
PUSH1 0x0e          // [0x0e,msg.value == 0,msg.value]
JUMPI               // [msg.value] --> if second value in stack is 1, jump to 0x0e program counter
PUSH0               // [0x00,msg.value]
DUP1                // [0x00,0x00,msg.value]
REVERT              // [msg.value]


// jump here if msg.value is 0
// put the runtime code on chain
JUMPDEST            // [msg.value]
POP                 // []
PUSH1 0xa5          // [0xa5]
DUP1                // [0xa5,0xa5]
PUSH2 0x001b        // [0x001b,0xa5,0xa5]
PUSH0               // [0x00,0x001b,0xa5,0xa5]
CODECOPY            // [0xa5] --> memory: [runtime_code]
PUSH0               // [0x00,0xa5]
RETURN              // [] --> will return the data at slot 0xa5 in memory
INVALID             // [] 

// here start the code used in codeCopy
// free memory pointer 
// that's the entry point of all calls
PUSH1 0x80
PUSH1 0x40
MSTORE

// check msg.value
CALLVALUE           // [msg.value]
DUP1                // [msg.value,msg.value]
ISZERO              // [[msg.value == 0,msg.value]
PUSH1 0x0e          // [0x0e, msg.value == 0,msg.value]
JUMPI               // [msg.value]
PUSH0               // [0x00, msg.value]
DUP1                // [0x00,0x00,msg.value]
REVERT              // [msg.value]

// if msg.value == 0 it will go 
// checks if calldata is big enough to contain a function selector
JUMPDEST            // [msg.value]
POP                 // [] 
PUSH1 0x04          // [0x04] 
CALLDATASIZE        // [calldata_size,0x04] 
LT                  // [calldata_size<0x04 ? 1 : 0] 
PUSH1 0x30          // [0x30,calldata_size<0x04 ? 1 : 0] --> will revert if calldata is too small
JUMPI               // []

// dispatcher
PUSH0               // [0x00]
CALLDATALOAD        // [calldata]
PUSH1 0xe0          // [0xe0,calldata_with_selector]
SHR                 // [selector] --> separate calldata from selector
DUP1                // [selector,selector]
PUSH4 0xcdfead2e    // [0xcdfead2e,selector,selector]
EQ                  // [0xcdfead2e == selector? 0:1, selector]
PUSH1 0x34          // [0x34, 0:1, selector]
JUMPI               // [selector]
DUP1                // [selector,selector]
PUSH4 0xe026c017    // [0xe026c017,selector,selector]
EQ                  // [0xe026c017 == selector ? 0:1,selector]
PUSH1 0x45          // [0x45,, 0:1, selector]
JUMPI               // [selector]

// 0x30 jump destination (revert)
JUMPDEST            // []
PUSH0               // [0x00]
DUP1                // [0x00,0x00]
REVERT              // []

// 0x34 jumpi
JUMPDEST            // [selector]
PUSH1 0x43          // [0x43,selector]
PUSH1 0x3f          // [0x3f,0x43,selector]
CALLDATASIZE        // [calldata_size,0x3f,0x43,selector]
PUSH1 0x04          // [0x04,calldata_size,0x3f,0x43,selector]
PUSH1 0x59          // [0x59,0x04,calldata_size,0x3f,0x43,selector]
JUMP                // [0x04,calldata_size,0x3f,0x43,selector]

// jump dest 0x3f
JUMPDEST            // [calldata_no_selector,0x43,selector]
PUSH0               // [0x00,calldata_no_selector,0x43,selector]
SSTORE              // [0x00,calldata_no_selector,0x43,selector] --> storage: 0x00 --> calldata
JUMP                // [selector]

// 0x43 jump destination
JUMPDEST            // [selector]
STOP                // [selector]

// this has to be the readNumberOfHorses function
JUMPDEST            // [selector]
PUSH0               // [0x00, selector]
SLOAD               // [value, selector]
PUSH1 0x40          // [0x40,value,selector]
MLOAD               // [0x80,value, selector] - memory : [0x40->0x80]
SWAP1               // [value,0x80,selector]
DUP2                // [0x80,value,0x80,selector]
MSTORE              // [0x80,selector] - memory : 0x80 -> value
PUSH1 0x20          // [0x20,0x80,selector]
ADD                 // [0xa0,selector]
PUSH1 0x40          // [0x40,0xa0,selector]
MLOAD               // [0x80,0xa0,selector]
DUP1                // [0x80,0x80,0xa0,selector]
SWAP2               // [0xa0,0x80,0x80,selector]
SUB                 // [0x20,0x80,selector]
SWAP1               // [0x80,0x20,selector]
RETURN              // [selector] --> return value of memory at slot 0x80

// 0x59 jump destination (updateHorseNumber)
// check to see if there is a value to update the horse number
JUMPDEST            // [0x04,calldata_size,0x3f,0x43,selector]
PUSH0               // [0x00,0x04,calldata_size,0x3f,0x43,selector]
PUSH1 0x20          // [0x20,0x00,0x04,calldata_size,0x3f,0x43,selector]
DUP3                // [0x04,0x20,0x00,0x04,calldata_size,0x3f,0x43,selector] --> duplicate 3rd item in stack
DUP5                // [calldata_size,0x04,0x20,0x00,0x04,calldata_size,0x3f,0x43,selector] 
SUB                 // [calldata_size - 0x04,0x20,0x00,0x04,calldata_size,0x3f,0x43,selector] 
SLT                 // [calldata_size - 0x04 < 0x20 ? 1:0,0x00,0x04,calldata_size,0x3f,0x43,selector]
ISZERO              // [calldata_size - 0x04 < 0x20 ? 1:0,0x00,0x04,calldata_size,0x3f,0x43,selector]
PUSH1 0x68          // [0x68,0,0x00,0x04,calldata_size,0x3f,0x43,selector]
JUMPI               // [0x00,0x04,calldata_size,0x3f,0x43,selector]
// revert if there is not enough calldata
PUSH0               // [0x00,0x00,0x04,calldata_size,0x3f,0x43,selector]
DUP1                // [0x00,0x00,0x00,0x04,calldata_size,0x3f,0x43,selector]
REVERT              // [0x20,0x00,0x04,calldata_size,0x3f,0x43,selector]

// jump destination 0x68
JUMPDEST            // [0x00,0x04,calldata_size,0x3f,0x43,selector]
POP                 // [0x04,calldata_size,0x3f,0x43,selector]
CALLDATALOAD        // [calldata_no_selector,calldata_size,0x3f,0x43,selector]
SWAP2               // [0x3f,calldata_size,calldata_no_selector,0x43,selector]
SWAP1               // [calldata_size,0x3f,calldata_no_selector,0x43,selector]
POP                 // [0x3f,calldata_no_selector,0x43,selector]
JUMP                // [calldata_no_selector,0x43,selector]

// metadata
INVALID
LOG2
PUSH5 0x6970667358
INVALID
SLT
KECCAK256
SWAP8
PUSH32 0x8652cc5392ce6356aa33e0e73ee886e4c73f9a9fd33dbf834e70d0b6b7996473
PUSH16 0x6c63430008140033