/**
 * @file TestInnovationTracker.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class InnovationTracker.
 *
 */

#include "InnovationTracker.h"
#include "Hyperparams.hpp"
#include "CatchConfig.hpp"
#include "NodeGenerationRecord.hpp"
#include "TestUtilities.hpp"
#include "include/catch.hpp"



TEST_CASE("Init test", "[InnovationTracker]"){
    resetNEAT(2, 2);
    InnovationTracker& tracker = NEAT::getInstance().getInnovationTracker();

    NodeGenerationRecord record = tracker.getNodeIdValueFromSplit(0);

    REQUIRE(record.node_id == 5);
    REQUIRE(record.innovation_in == 6);
    REQUIRE(record.innovation_out == 7);

    REQUIRE(tracker.getInnovationValueFromNodes(5, 3) == 8);
}

TEST_CASE("getInnovationValueFromNodes cases test", "[InnovationTracker]"){
    resetNEAT(2, 2);
    InnovationTracker& tracker = NEAT::getInstance().getInnovationTracker();

    unsigned int id_1 = tracker.getInnovationValueFromNodes(4, 5);
    unsigned int id_2 = tracker.getInnovationValueFromNodes(5, 6);
    unsigned int id_3 = tracker.getInnovationValueFromNodes(4, 5);
    unsigned int id_4 = tracker.getInnovationValueFromNodes(5, 6);

    REQUIRE(id_1 == 6);
    REQUIRE(id_2 == 7);
    REQUIRE(id_1 == id_3);
    REQUIRE(id_2 == id_4);
}

TEST_CASE("getNodeIdValueFromSplit cases test.", "[InnovationTracker]") {
    resetNEAT(2, 2);
    InnovationTracker& tracker = NEAT::getInstance().getInnovationTracker();

    NodeGenerationRecord record_1 = tracker.getNodeIdValueFromSplit(0);
    NodeGenerationRecord record_2 = tracker.getNodeIdValueFromSplit(7);
    NodeGenerationRecord record_3 = tracker.getNodeIdValueFromSplit(0);
    NodeGenerationRecord record_4 = tracker.getNodeIdValueFromSplit(7);

    REQUIRE(record_1 == NodeGenerationRecord{5, 6, 7});
    REQUIRE(record_2 == NodeGenerationRecord{6, 8, 9});
    REQUIRE(record_1 == record_3);
    REQUIRE(record_2 == record_4);

}