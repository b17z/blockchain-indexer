#include <fstream>
#include <vector>

/*  VTC Blockindexer - A utility to build additional indexes to the 
    Vertcoin blockchain by scanning and indexing the blockfiles
    downloaded by Vertcoin Core.
    
    Copyright (C) 2017  Gert-Jaap Glasbergen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <restbed>
#include <jsonrpccpp/client/connectors/httpclient.h>

#include "leveldb/db.h"
#include "leveldb/write_batch.h"

#include "vertcoinrpc.h"
#include "blockreader.h"
#include "scriptsolver.h"
#include "mempoolmonitor.h"

using namespace std;
using namespace restbed;

namespace VtcBlockIndexer {
    
    /**
     * The HttpServer class contains the methods used to run the HTTP public interface for
     * querying the blockchain.
     */
    
    class HttpServer {
        public:
            HttpServer(const shared_ptr<leveldb::DB> db, const shared_ptr<VtcBlockIndexer::MempoolMonitor> mempoolMonitor, string blocksDir);
            void run();
            /* REST Api for returning the balance of a given address */
            void addressBalance( const shared_ptr< Session > session );

            /* REST Api for returning the TXOs on a given address */
            void addressTxos( const shared_ptr< Session > session );
            
            /* REST Api for returning the transaction details with a given hash */
            void getTransaction(const shared_ptr<Session> session);
            
            /* REST Api for returning the transaction proof for a given hash */
            void getTransactionProof(const shared_ptr<Session> session);
            
            /* REST Api for returning if a given outpoint is spent (and if so, which TX spends it) */
            void outpointSpend( const shared_ptr< Session > session );
            
            /* REST Api for returning if a collection of given outpoints is spent (and if so, which TX spends it) */
            void outpointSpends( const shared_ptr< Session > session );
            
            /* REST Api for returning list of blocks */
            void getBlocks( const shared_ptr< Session > session );
            
             /* REST Api for returning list of blocks */
            void getBlock( const shared_ptr< Session > session );
            
            /* REST Api for returning list of blocks by date */
            void getBlocksByDate( const shared_ptr< Session > session );

            /* REST Api for returning the TXs inside a block */
            void getBlockTransactions( const shared_ptr< Session > session );
            
            /* REST Api for returning list of transactionids in the memory pool */
            void mempoolTransactionIds( const shared_ptr< Session > session );
            
            /* REST Api for returning sync status */
            void sync( const shared_ptr< Session > session );

            vector<string> getAddressesForTxo(string txHash, uint64_t idx);
            uint64_t getValueForTxo(string txHash, uint64_t idx);

            /* REST Api for sending a hex transaction on the VTC p2p network*/
            void sendRawTransaction( const shared_ptr< Session > session );
            
        private:
            shared_ptr<leveldb::DB> db;
            unique_ptr<VertcoinClient> vertcoind;
            unique_ptr<jsonrpc::HttpClient> httpClient;
            unique_ptr<VtcBlockIndexer::BlockReader> blockReader;
            unique_ptr<VtcBlockIndexer::ScriptSolver> scriptSolver;
            shared_ptr<VtcBlockIndexer::MempoolMonitor> mempoolMonitor;
            /** Directory containing the blocks
             */
            string blocksDir; 

    };
}