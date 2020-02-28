# RPC Interface

Conclave's RPC interface allows clients to securely interact with both the Bitcoin and Conclave network.

## Wrapped Responses

Every response from the RPC interface is wrapped in an outer object which contains the following fields:

* *signature* (string) - Signature signing the entire response including the remaining fields in in the response
 wrapper.
* *responder* (string) - NodeID of the responding node.
* *requestHash* (string) - Hash of the request.
* *latestBlock* (object) - Object containing information on the most recent Bitcoin block. Contains the following
 fields:
    * *time* (string) - The timestamp of the block. This is not always accurate.
    * *hash* (string) - The block header's hash.
    * *height* (number) - The block's height

## Methods

* [GetAddressBalance](methods/GetAddressBalance.md)
* [NodeInfo](methods/NodeInfo.md)
* [MakeEntryTx](methods/MakeEntryTx.md)
