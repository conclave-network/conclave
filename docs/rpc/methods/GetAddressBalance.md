# GetAddressBalance

**Method Name**: GetAddressBalance

**Arguments**: 
* *address* (string) - The address to get the balance of. Can be one of either: P2PKH, P2SH, P2WPKH, P2WSH, P2CPKH
, P2CSH

**Return Value**: A JSON object with the following fields:
* *balance* (number) - The balance of the address in satoshis.

## Usage

## Example
```
curl -X POST -d '{"method": "GetAddressBalance", "params": {"address": "1FeexV6bAHb8ybZjqQMjJrcCrHGW9sb6uF"}}' http://127.0.0.1:8008/
{
  "signature": "3045022100d769891e807d355c429c567451aeeb0f0e8a142723d22977027037ce0d7f0fd402200847ec48547046b016c1665e70c27481f0c5d4f47b8df3c425b4d52cc9abed3a",
  "responder": "021d5219a13f0f23rbbd8e88abe9ab9eac77f9daaa859cfff0580279a15d9aa12b",
  "requestHash": "40e010bac965598bcacb8bd633f9e592b7bc58282a2622b6db612f6b12391679",
  "latestBlock": {
    "time": "2020-02-06T20:43:57Z",
    "hash": "0000000000000000000fa2ceca4de4717f070c6b02dee2e47fd5f9cda5f1b3a6",
    "height": 616284
  },
  "response": {
    "balance": 7995720153741
  }
}
```
