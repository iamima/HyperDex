// Copyright (c) 2012, Cornell University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of HyperDex nor the names of its contributors may be
//       used to endorse or promote products derived from this software without
//       specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#define __STDC_LIMIT_MACROS

// Google Test
#include <gtest/gtest.h>

// STL
#include <algorithm>

// HyperDex
#include "disk/cuckoo_index.h"

#pragma GCC diagnostic ignored "-Wswitch-default"

using namespace hyperdex;

namespace
{

TEST(CuckooIndexTest, CtorDtor)
{
    cuckoo_index ct;
}

TEST(CuckooIndexTest, Eviction1)
{
    cuckoo_index ct;
    cuckoo_returncode rc;
    std::vector<uint64_t> vals;

    // Insert a new entry
    rc = ct.insert(0xdeadbeef00000000ULL, 0, 0x010102010301ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeef00000000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010102010301ULL, vals[0]);

    // Now fill up the bucket in the first table
    rc = ct.insert(0xdeadbeef00010000ULL, 0, 0x010202020302ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeef00010000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010202020302ULL, vals[0]);
    rc = ct.insert(0xdeadbeef00020000ULL, 0, 0x010302030303ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeef00020000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010302030303ULL, vals[0]);
    rc = ct.insert(0xdeadbeef00030000ULL, 0, 0x010402040304ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeef00030000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010402040304ULL, vals[0]);
    rc = ct.insert(0xdeadbeef00040000ULL, 0, 0x010502050305ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeef00040000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010502050305ULL, vals[0]);

    // At this point, the 00 bucket in the first table is filled with 5 entries,
    // each of which goes to a different bucket (00-04) in the second table.
    // In the following rounds, we bump things to the other buckets

    // round 1
    rc = ct.insert(0xcafebabe00000000ULL, 0, 0x010602060306ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xcafebabe00000000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010602060306ULL, vals[0]);
    rc = ct.insert(0xcafebabe00010000ULL, 0, 0x010702070307ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xcafebabe00010000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010702070307ULL, vals[0]);
    rc = ct.insert(0xcafebabe00020000ULL, 0, 0x010802080308ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xcafebabe00020000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010802080308ULL, vals[0]);
    rc = ct.insert(0xcafebabe00030000ULL, 0, 0x010902090309ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xcafebabe00030000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010902090309ULL, vals[0]);
    rc = ct.insert(0xcafebabe00040000ULL, 0, 0x010a020a030aULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xcafebabe00040000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010a020a030aULL, vals[0]);
    // round 2
    rc = ct.insert(0x8badf00d00000000ULL, 0, 0x010b020b030bULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x8badf00d00000000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010b020b030bULL, vals[0]);
    rc = ct.insert(0x8badf00d00010000ULL, 0, 0x010c020c030cULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x8badf00d00010000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010c020c030cULL, vals[0]);
    rc = ct.insert(0x8badf00d00020000ULL, 0, 0x010d020d030dULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x8badf00d00020000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010d020d030dULL, vals[0]);
    rc = ct.insert(0x8badf00d00030000ULL, 0, 0x010e020e030eULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x8badf00d00030000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010e020e030eULL, vals[0]);
    rc = ct.insert(0x8badf00d00040000ULL, 0, 0x010f020f030fULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x8badf00d00040000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010f020f030fULL, vals[0]);
    // round 3
    rc = ct.insert(0x1eaff00d00000000ULL, 0, 0x011002100310ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x1eaff00d00000000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011002100310ULL, vals[0]);
    rc = ct.insert(0x1eaff00d00010000ULL, 0, 0x011102110311ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x1eaff00d00010000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011102110311ULL, vals[0]);
    rc = ct.insert(0x1eaff00d00020000ULL, 0, 0x011202120312ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x1eaff00d00020000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011202120312ULL, vals[0]);
    rc = ct.insert(0x1eaff00d00030000ULL, 0, 0x011302130313ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x1eaff00d00030000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011302130313ULL, vals[0]);
    rc = ct.insert(0x1eaff00d00040000ULL, 0, 0x011402140314ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x1eaff00d00040000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011402140314ULL, vals[0]);
    // round 4
    rc = ct.insert(0xfaceb00c00000000ULL, 0, 0x011502150315ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfaceb00c00000000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011502150315ULL, vals[0]);
    rc = ct.insert(0xfaceb00c00010000ULL, 0, 0x011602160316ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfaceb00c00010000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011602160316ULL, vals[0]);
    rc = ct.insert(0xfaceb00c00020000ULL, 0, 0x011702170317ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfaceb00c00020000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011702170317ULL, vals[0]);
    rc = ct.insert(0xfaceb00c00030000ULL, 0, 0x011802180318ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfaceb00c00030000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011802180318ULL, vals[0]);
    rc = ct.insert(0xfaceb00c00040000ULL, 0, 0x011902190319ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfaceb00c00040000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011902190319ULL, vals[0]);
    // round 5
    rc = ct.insert(0xfacefeed00000000ULL, 0, 0x011a021a031aULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfacefeed00000000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011a021a031aULL, vals[0]);
    rc = ct.insert(0xfacefeed00010000ULL, 0, 0x011b021b031bULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfacefeed00010000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011b021b031bULL, vals[0]);
    rc = ct.insert(0xfacefeed00020000ULL, 0, 0x011c021c031cULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfacefeed00020000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011c021c031cULL, vals[0]);
    rc = ct.insert(0xfacefeed00030000ULL, 0, 0x011d021d031dULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfacefeed00030000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011d021d031dULL, vals[0]);
    rc = ct.insert(0xfacefeed00040000ULL, 0, 0x011e021e031eULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfacefeed00040000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011e021e031eULL, vals[0]);
}

TEST(CuckooIndexTest, Eviction2)
{
    cuckoo_index ct;
    cuckoo_returncode rc;
    std::vector<uint64_t> vals;

    // Insert a new entry
    rc = ct.insert(0xdeadbeef00000000ULL, 0, 0x010102010301ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeef00000000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010102010301ULL, vals[0]);

    // Now fill up the bucket in the first table
    rc = ct.insert(0xcafebabe00000000ULL, 0, 0x010602060306ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xcafebabe00000000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010602060306ULL, vals[0]);
    rc = ct.insert(0x8badf00d00000000ULL, 0, 0x010b020b030bULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x8badf00d00000000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010b020b030bULL, vals[0]);
    rc = ct.insert(0x1eaff00d00000000ULL, 0, 0x011002100310ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x1eaff00d00000000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011002100310ULL, vals[0]);
    rc = ct.insert(0xfaceb00c00000000ULL, 0, 0x011502150315ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfaceb00c00000000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011502150315ULL, vals[0]);
    rc = ct.insert(0xfacefeed00000000ULL, 0, 0x011a021a031aULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfacefeed00000000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011a021a031aULL, vals[0]);

    // At this point, the 00 bucket in the first table is filled with 5 entries,
    // each of which goes to a different bucket (00-04) in the second table.
    // In the following rounds, we bump things to the other buckets

    // round 1
    rc = ct.insert(0xdeadbeef00010000ULL, 0, 0x010202020302ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeef00010000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010202020302ULL, vals[0]);
    rc = ct.insert(0xcafebabe00010000ULL, 0, 0x010702070307ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xcafebabe00010000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010702070307ULL, vals[0]);
    rc = ct.insert(0x8badf00d00010000ULL, 0, 0x010c020c030cULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x8badf00d00010000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010c020c030cULL, vals[0]);
    rc = ct.insert(0x1eaff00d00010000ULL, 0, 0x011102110311ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x1eaff00d00010000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011102110311ULL, vals[0]);
    rc = ct.insert(0xfaceb00c00010000ULL, 0, 0x011602160316ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfaceb00c00010000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011602160316ULL, vals[0]);
    rc = ct.insert(0xfacefeed00010000ULL, 0, 0x011b021b031bULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfacefeed00010000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011b021b031bULL, vals[0]);
    // round 2
    rc = ct.insert(0xdeadbeef00020000ULL, 0, 0x010302030303ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeef00020000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010302030303ULL, vals[0]);
    rc = ct.insert(0xcafebabe00020000ULL, 0, 0x010802080308ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xcafebabe00020000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010802080308ULL, vals[0]);
    rc = ct.insert(0x8badf00d00020000ULL, 0, 0x010d020d030dULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x8badf00d00020000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010d020d030dULL, vals[0]);
    rc = ct.insert(0x1eaff00d00020000ULL, 0, 0x011202120312ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x1eaff00d00020000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011202120312ULL, vals[0]);
    rc = ct.insert(0xfaceb00c00020000ULL, 0, 0x011702170317ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfaceb00c00020000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011702170317ULL, vals[0]);
    rc = ct.insert(0xfacefeed00020000ULL, 0, 0x011c021c031cULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfacefeed00020000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011c021c031cULL, vals[0]);
    // round 3
    rc = ct.insert(0xdeadbeef00030000ULL, 0, 0x010402040304ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeef00030000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010402040304ULL, vals[0]);
    rc = ct.insert(0xcafebabe00030000ULL, 0, 0x010902090309ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xcafebabe00030000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010902090309ULL, vals[0]);
    rc = ct.insert(0x8badf00d00030000ULL, 0, 0x010e020e030eULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x8badf00d00030000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010e020e030eULL, vals[0]);
    rc = ct.insert(0x1eaff00d00030000ULL, 0, 0x011302130313ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x1eaff00d00030000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011302130313ULL, vals[0]);
    rc = ct.insert(0xfaceb00c00030000ULL, 0, 0x011802180318ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfaceb00c00030000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011802180318ULL, vals[0]);
    rc = ct.insert(0xfacefeed00030000ULL, 0, 0x011d021d031dULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfacefeed00030000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011d021d031dULL, vals[0]);
    // round 4
    rc = ct.insert(0xdeadbeef00040000ULL, 0, 0x010502050305ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeef00040000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010502050305ULL, vals[0]);
    rc = ct.insert(0xcafebabe00040000ULL, 0, 0x010a020a030aULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xcafebabe00040000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010a020a030aULL, vals[0]);
    rc = ct.insert(0x8badf00d00040000ULL, 0, 0x010f020f030fULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x8badf00d00040000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010f020f030fULL, vals[0]);
    rc = ct.insert(0x1eaff00d00040000ULL, 0, 0x011402140314ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0x1eaff00d00040000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011402140314ULL, vals[0]);
    rc = ct.insert(0xfaceb00c00040000ULL, 0, 0x011902190319ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfaceb00c00040000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011902190319ULL, vals[0]);
    rc = ct.insert(0xfacefeed00040000ULL, 0, 0x011e021e031eULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xfacefeed00040000ULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x011e021e031eULL, vals[0]);
}

TEST(CuckooIndexTest, RemoveOldest)
{
    cuckoo_index ct;
    cuckoo_returncode rc;
    std::vector<uint64_t> vals;

    // Insert ten new entries with the same key, checking each time that they
    // match what we expect
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x010101010101ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x020202020202ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(2, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x030303030303ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(3, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x040404040404ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(4, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x050505050505ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(5, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x060606060606ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(6, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x070707070707ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(7, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x080808080808ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(8, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    ASSERT_EQ(0x080808080808ULL, vals[7]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x090909090909ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(9, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    ASSERT_EQ(0x080808080808ULL, vals[7]);
    ASSERT_EQ(0x090909090909ULL, vals[8]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x0a0a0a0a0a0aULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(10, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    ASSERT_EQ(0x080808080808ULL, vals[7]);
    ASSERT_EQ(0x090909090909ULL, vals[8]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[9]);

    // Now remove them
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x010101010101ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x010101010101ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(9, vals.size());
    ASSERT_EQ(0x020202020202ULL, vals[0]);
    ASSERT_EQ(0x030303030303ULL, vals[1]);
    ASSERT_EQ(0x040404040404ULL, vals[2]);
    ASSERT_EQ(0x050505050505ULL, vals[3]);
    ASSERT_EQ(0x060606060606ULL, vals[4]);
    ASSERT_EQ(0x070707070707ULL, vals[5]);
    ASSERT_EQ(0x080808080808ULL, vals[6]);
    ASSERT_EQ(0x090909090909ULL, vals[7]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[8]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x020202020202ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x020202020202ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(8, vals.size());
    ASSERT_EQ(0x030303030303ULL, vals[0]);
    ASSERT_EQ(0x040404040404ULL, vals[1]);
    ASSERT_EQ(0x050505050505ULL, vals[2]);
    ASSERT_EQ(0x060606060606ULL, vals[3]);
    ASSERT_EQ(0x070707070707ULL, vals[4]);
    ASSERT_EQ(0x080808080808ULL, vals[5]);
    ASSERT_EQ(0x090909090909ULL, vals[6]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[7]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x030303030303ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x030303030303ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(7, vals.size());
    ASSERT_EQ(0x040404040404ULL, vals[0]);
    ASSERT_EQ(0x050505050505ULL, vals[1]);
    ASSERT_EQ(0x060606060606ULL, vals[2]);
    ASSERT_EQ(0x070707070707ULL, vals[3]);
    ASSERT_EQ(0x080808080808ULL, vals[4]);
    ASSERT_EQ(0x090909090909ULL, vals[5]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[6]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x040404040404ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x040404040404ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(6, vals.size());
    ASSERT_EQ(0x050505050505ULL, vals[0]);
    ASSERT_EQ(0x060606060606ULL, vals[1]);
    ASSERT_EQ(0x070707070707ULL, vals[2]);
    ASSERT_EQ(0x080808080808ULL, vals[3]);
    ASSERT_EQ(0x090909090909ULL, vals[4]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[5]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x050505050505ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x050505050505ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(5, vals.size());
    ASSERT_EQ(0x060606060606ULL, vals[0]);
    ASSERT_EQ(0x070707070707ULL, vals[1]);
    ASSERT_EQ(0x080808080808ULL, vals[2]);
    ASSERT_EQ(0x090909090909ULL, vals[3]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[4]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x060606060606ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x060606060606ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(4, vals.size());
    ASSERT_EQ(0x070707070707ULL, vals[0]);
    ASSERT_EQ(0x080808080808ULL, vals[1]);
    ASSERT_EQ(0x090909090909ULL, vals[2]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[3]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x070707070707ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x070707070707ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(3, vals.size());
    ASSERT_EQ(0x080808080808ULL, vals[0]);
    ASSERT_EQ(0x090909090909ULL, vals[1]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[2]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x080808080808ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x080808080808ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(2, vals.size());
    ASSERT_EQ(0x090909090909ULL, vals[0]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[1]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x090909090909ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x090909090909ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[0]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x0a0a0a0a0a0aULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x0a0a0a0a0a0aULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(NOT_FOUND, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(0, vals.size());
}

TEST(CuckooIndexTest, RemoveNewest)
{
    cuckoo_index ct;
    cuckoo_returncode rc;
    std::vector<uint64_t> vals;

    // Insert ten new entries with the same key, checking each time that they
    // match what we expect
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x010101010101ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x020202020202ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(2, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x030303030303ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(3, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x040404040404ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(4, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x050505050505ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(5, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x060606060606ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(6, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x070707070707ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(7, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x080808080808ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(8, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    ASSERT_EQ(0x080808080808ULL, vals[7]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x090909090909ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(9, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    ASSERT_EQ(0x080808080808ULL, vals[7]);
    ASSERT_EQ(0x090909090909ULL, vals[8]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x0a0a0a0a0a0aULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(10, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    ASSERT_EQ(0x080808080808ULL, vals[7]);
    ASSERT_EQ(0x090909090909ULL, vals[8]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[9]);

    // Now remove them
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x0a0a0a0a0a0aULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x0a0a0a0a0a0aULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(9, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    ASSERT_EQ(0x080808080808ULL, vals[7]);
    ASSERT_EQ(0x090909090909ULL, vals[8]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x090909090909ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x090909090909ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(8, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    ASSERT_EQ(0x080808080808ULL, vals[7]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x080808080808ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x080808080808ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(7, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x070707070707ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x070707070707ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(6, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x060606060606ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x060606060606ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(5, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x050505050505ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x050505050505ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(4, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x040404040404ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x040404040404ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(3, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x030303030303ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x030303030303ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(2, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x020202020202ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x020202020202ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x010101010101ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x010101010101ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(NOT_FOUND, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(0, vals.size());
}

TEST(CuckooIndexTest, RemoveMiddle)
{
    cuckoo_index ct;
    cuckoo_returncode rc;
    std::vector<uint64_t> vals;

    // Insert ten new entries with the same key, checking each time that they
    // match what we expect
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x010101010101ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x020202020202ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(2, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x030303030303ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(3, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x040404040404ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(4, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x050505050505ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(5, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x060606060606ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(6, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x070707070707ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(7, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x080808080808ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(8, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    ASSERT_EQ(0x080808080808ULL, vals[7]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x090909090909ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(9, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    ASSERT_EQ(0x080808080808ULL, vals[7]);
    ASSERT_EQ(0x090909090909ULL, vals[8]);
    rc = ct.insert(0xdeadbeeffacefeedULL, 0, 0x0a0a0a0a0a0aULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(10, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x050505050505ULL, vals[4]);
    ASSERT_EQ(0x060606060606ULL, vals[5]);
    ASSERT_EQ(0x070707070707ULL, vals[6]);
    ASSERT_EQ(0x080808080808ULL, vals[7]);
    ASSERT_EQ(0x090909090909ULL, vals[8]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[9]);

    // Now remove them
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x050505050505ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x050505050505ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(9, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x060606060606ULL, vals[4]);
    ASSERT_EQ(0x070707070707ULL, vals[5]);
    ASSERT_EQ(0x080808080808ULL, vals[6]);
    ASSERT_EQ(0x090909090909ULL, vals[7]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[8]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x060606060606ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x060606060606ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(8, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x040404040404ULL, vals[3]);
    ASSERT_EQ(0x070707070707ULL, vals[4]);
    ASSERT_EQ(0x080808080808ULL, vals[5]);
    ASSERT_EQ(0x090909090909ULL, vals[6]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[7]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x040404040404ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x040404040404ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(7, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x070707070707ULL, vals[3]);
    ASSERT_EQ(0x080808080808ULL, vals[4]);
    ASSERT_EQ(0x090909090909ULL, vals[5]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[6]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x070707070707ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x070707070707ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(6, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x030303030303ULL, vals[2]);
    ASSERT_EQ(0x080808080808ULL, vals[3]);
    ASSERT_EQ(0x090909090909ULL, vals[4]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[5]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x030303030303ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x030303030303ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(5, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x080808080808ULL, vals[2]);
    ASSERT_EQ(0x090909090909ULL, vals[3]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[4]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x080808080808ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x080808080808ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(4, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x020202020202ULL, vals[1]);
    ASSERT_EQ(0x090909090909ULL, vals[2]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[3]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x020202020202ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x020202020202ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(3, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x090909090909ULL, vals[1]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[2]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x090909090909ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x090909090909ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(2, vals.size());
    ASSERT_EQ(0x010101010101ULL, vals[0]);
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[1]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x010101010101ULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x010101010101ULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(SUCCESS, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(1, vals.size());
    ASSERT_EQ(0x0a0a0a0a0a0aULL, vals[0]);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x0a0a0a0a0a0aULL);
    ASSERT_EQ(SUCCESS, rc);
    rc = ct.remove(0xdeadbeeffacefeedULL, 0x0a0a0a0a0a0aULL);
    ASSERT_EQ(NOT_FOUND, rc);
    rc = ct.lookup(0xdeadbeeffacefeedULL, &vals);
    ASSERT_EQ(NOT_FOUND, rc);
    std::sort(vals.begin(), vals.end());
    ASSERT_EQ(0, vals.size());
}

TEST(CuckooIndexTest, OneMillionInserts)
{
#define ONE_MILLION 1000000
    cuckoo_index ct;
    cuckoo_returncode rc;

    for (uint64_t i = 1; i <= ONE_MILLION; ++i)
    {
        rc = ct.insert(i, 0, 0xdeadbeef + i);
        ASSERT_EQ(SUCCESS, rc);
    }

    for (uint64_t i = 1; i <= ONE_MILLION; ++i)
    {
        std::vector<uint64_t> vals;
        rc = ct.lookup(i, &vals);
        ASSERT_EQ(SUCCESS, rc);
    }

    for (uint64_t i = 1; i <= ONE_MILLION; ++i)
    {
        rc = ct.remove(i, 0xdeadbeef + i);
        ASSERT_EQ(SUCCESS, rc);
    }

    for (uint64_t i = 1; i <= ONE_MILLION; ++i)
    {
        std::vector<uint64_t> vals;
        rc = ct.lookup(i, &vals);
        ASSERT_EQ(NOT_FOUND, rc);
    }

    for (uint64_t i = 1; i <= ONE_MILLION; ++i)
    {
        rc = ct.remove(i, 0xdeadbeef + i);
        ASSERT_EQ(NOT_FOUND, rc);
    }
}

} // namespace