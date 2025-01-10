// SPDX-License-Identifier: MIT
pragma solidity 0.8.20;

import {HorseStore} from "../../src/v1/HorseStore.sol";
import {HorseStoreYul} from "../../src/v1/HorseStoreYul.sol";
import {Test, console2} from "forge-std/Test.sol";

abstract contract Base_TestV1 is Test {
    string public constant horseStoreLocation = "v1/HorseStore";
    HorseStore horseStoreHuff;
    HorseStore horseStoreSol;
    HorseStoreYul horseStoreYul;

    function setUp() public virtual {
        horseStoreYul = new HorseStoreYul();
        horseStoreSol = new HorseStore();
    }

    function test_updateHorseNumber() public {
        //setUp
        //execution
        //assert
    }
}
