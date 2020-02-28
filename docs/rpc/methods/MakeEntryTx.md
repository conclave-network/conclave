# MakeEntryTx

**Method Name**: MakeEntryTx

**Arguments**: A JSON object with the following fields:

* *sources* (object) - Defines all the funding sources from the blockchain which will fund the transaction
. The following sources are defined:
    * *outpoints* (array of objects) - Outpoints from unspent transaction outputs (UTXOs) which will fund this entry
     transaction. Each outpoint has the following fields:
        * *txId* (string) - Transaction ID of transaction where UTXO appears.
        * *index* (number) - Zero-indexed index of the UTXO from the list of outputs in the transaction.
 * *destinations* (object) - Defines the destinations of all funds referenced in *Sources*. The following
  destinations are defined:
    * *conclave* (array of objects) - Destinations on the Conclave blockchain. Each destination has the following
     fields:
        * *address* (string) - Destination address. Should be either a P2CPKH or P2CSH address.
        * *value* (number) - The value in satoshis to send to the address
    * *bitcoin* (array of objects) - Destinations on the Bitcoin blockchain, typically used for "Change". Each
     destination has the following fields:
        * *address* (string) - Destination address. Should be either a P2PKH, P2SH, P2WPKH, or P2WSH address.
        * *value* (number) - The value in satoshis to send to the address
 
 **Return Value**: A JSON object with the following fields:
 
 * *bitcoinTx* (object) - A standard Bitcoin transaction which has been populated with:
    * *version* (number) - Bitcoin protocol version
    * *lockTime* (number) - Transaction lock time
    * Inputs referencing the UTXOs specified in the request.
    * Outputs which consist of:
        * A single P2WSH output with a value equal to the sum of Concalve destination outputs' values in the request.
        * Any change outputs specified in the request.
 * *conclaveEntryTx* (object) - A Conclave entry transaction consisting of the following fields:
    * *outputs* (array of objects) - Conclave blockchain outputs consisting of fields:
        * *scriptPubKey* (object) - destination script. Typically a P2PKH/P2SH script.
        * *value* (number) - The value in satoshis to send to the script.
 * *redeemScript* (array of strings) - Redeem script which contains PoT-compliant multisig and the hash of *conclaveTx*.
 
## Usage
  
## Example
 ```
$ cat makeEntryTxRequest.json
{
  "method": "MakeEntryTx",
  "params": {
    "sources": {
      "outpoints": [
        {
          "txId": "85d7ea748cafacf27524179834589793a11cc500f5f3d378bdf9801e484f62cc",
          "index": 1
        }
      ]
    },
    "destinations": {
      "conclave": [
        {
          "address": "5Xjy8xUQZpdnvzhLJmJktWjMwZ9hLHPG",
          "value": 50000
        }
      ],
      "bitcoin": [
        {
          "address": "1NxfhWzkuDre6nvG54n4KT82YLVdsziBCq",
          "value": 45000
        }
      ]
    }
  }
}

$ curl -X POST -d @makeEntryTxRequest.json http://127.0.0.1:8008/
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
    "bitcoinTx": {
      "version": 1,
      "lockTime": 0,
      "inputs": [
        {
          "outpoint": {
            "txId": "85d7ea748cafacf27524179834589793a11cc500f5f3d378bdf9801e484f62cc",
            "index": 1,
          },
          "sequence": 4294967295,
          "scriptSig": []
        }
      ],
      "outputs": [
        {
          "scriptPubKey": [
            "0",
            "a0f056fac0f3d949a348201fdfc9a3b6b9527e883ea5c1c1cf17e8d4a8aaa5c8"
          ],
          "value": 50000
        },
        { 
          "scriptPubKey": [
            "DUP",
            "HASH160",
            "e7b1d68cbda7de5d5eb448aa0d6cc01785065d27",
            "EQUALVERIFY",
            "CHECKSIG"
          ],
          "value": 45000
        }
      ]
    },
    "conclaveEntryTx": {
      "outputs": [
        {
          "scriptPubKey": [
            "DUP",
            "HASH160",
            "f0e1d2c3b4a5968778695a4b3c2d1e0ff0e1d2c3",
            "EQUALVERIFY",
            "CHECKSIG"
          ],
          "value": 50000
        }
      ]
    },
    "redeemScript": [
      "614811cd92661d3bd4b393363e6ce14e0b53592896d513de48ed0d8915b174c8",
      "DROP",
      "2",
      "03519e185b4c0de842cb5cf9e49cd6df00569c4f34bf87ed295b65f9473d508e9d",
      "02a013b162136d674a37b95ce50d0ae02c5f3d3ab74e52d363e809cde86e92cbbb",
      "031f74bdb5873d9ae3d46aa3473718eac8fe19d4b9d62d9421cf6b9b6beab1e6c4",
      "3",
      "CHECKSIG"
    ]
  }
}
 ```
