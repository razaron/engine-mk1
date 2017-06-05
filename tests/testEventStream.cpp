#include "EventStream.hpp"

#include <catch.hpp>

using namespace razaron::eventstream;

SCENARIO("You can push/pop Events from an EventStream")
{
    GIVEN("An EventStream with no incoming and some outgoing events")
    {
        EventStream stream;

        stream.pushEvent({0, EventType::TYPE_1, new std::string{"lolwut"}}, StreamType::OUTGOING);

        std::vector<Event> events;
        for (unsigned int i = 1; i < 5; i++)
        {
            Event e{i, EventType::TYPE_2, new std::string{"dis gun be gud"}};
            events.push_back(e);
        }

        stream.pushEvents(events, StreamType::OUTGOING);

        WHEN("Popping outgoing events")
        {
            std::vector<Event> events = stream.popEvents(StreamType::OUTGOING);

            REQUIRE(events.size() == 5);
            REQUIRE(*(static_cast<std::string *>(events[0].data)) == "lolwut");
            REQUIRE(*(static_cast<std::string *>(events[1].data)) == "dis gun be gud");
        }

        WHEN("Events are pushed onto the incoming stream")
        {
            for (unsigned int i = 1; i < 5; i++)
            {
                stream.pushEvent({i, EventType::TYPE_2, new std::string{"get in dere"}}, StreamType::INCOMING);
            }

            std::vector<Event> events = stream.popEvents(StreamType::INCOMING);

            REQUIRE(events.size() == 4);
            REQUIRE(*(static_cast<std::string *>(events[0].data)) == "get in dere");
        }
    }
}

SCENARIO("EventStreams can propogate Events to eachother")
{
    GIVEN("An EventStream with outgoing events and an empty EventStream")
    {
        EventStream a;
        EventStream b;

        a.pushEvent({0, EventType::TYPE_1, new std::string{"lolwut"}}, StreamType::OUTGOING);

        std::vector<Event> events;
        for (unsigned int i = 1; i < 5; i++)
        {
            Event e{i, EventType::TYPE_1, new std::string{"dis gun be gud"}};
            events.push_back(e);
        }

        a.pushEvents(events, StreamType::OUTGOING);

        WHEN("a bubbles Events to b")
        {
            a.propogateEvents(&b);

            std::vector<Event> events = b.popEvents(StreamType::INCOMING);

            REQUIRE(events.size() == 5);
            REQUIRE(*(static_cast<std::string *>(events[0].data)) == "lolwut");
            REQUIRE(*(static_cast<std::string *>(events[1].data)) == "dis gun be gud");
        }
    }
}
