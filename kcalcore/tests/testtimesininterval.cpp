/*
  This file is part of the kcalcore library.

  Copyright (C) 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>
  Author: Sergio Martins <sergio.martins@kdab.com>
  Copyright (C) 2019 Open Mobile Platform LLC

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "testtimesininterval.h"
#include "../event.h"

#include <qtest_kde.h>
QTEST_KDEMAIN( TimesInIntervalTest, NoGUI )

using namespace KCalCore;

void TimesInIntervalTest::test()
{
  const KDateTime currentDate( QDate::currentDate() );
  Event *event = new Event();
  event->setDtStart( currentDate );
  event->setDtEnd( currentDate.addDays( 1 ) );
  event->setAllDay( true );
  event->setSummary( "Event1 Summary" );

  event->recurrence()->setDaily( 1 );

  //------------------------------------------------------------------------------------------------
  // Just to warm up
  QVERIFY( event->recurs() );
  QVERIFY( event->recursAt( currentDate ) );

  //------------------------------------------------------------------------------------------------
  // Daily recurrence that never stops.
  // Should return numDaysInInterval+1 occurrences
  const int numDaysInInterval = 7;
  KDateTime start( currentDate );
  KDateTime end( start.addDays( numDaysInInterval ) );

  start.setTime( QTime( 0, 0, 0 ) );
  end.setTime( QTime( 23, 59, 59 ) );
  DateTimeList dateList = event->recurrence()->timesInInterval( start, end );
  QVERIFY( dateList.count() == numDaysInInterval + 1 );

  //------------------------------------------------------------------------------------------------
  // start == end == first day of the recurrence, should only return 1 occurrence
  end = start;
  end.setTime( QTime( 23, 59, 59 ) );
  dateList = event->recurrence()->timesInInterval( start, end );
  QVERIFY( dateList.count() == 1 );

  //------------------------------------------------------------------------------------------------
  // Test daily recurrence that only lasts X days
  const int recurrenceDuration = 3;
  event->recurrence()->setDuration( recurrenceDuration );
  end = start.addDays( 100 );
  dateList = event->recurrence()->timesInInterval( start, end );
  QVERIFY( dateList.count() == recurrenceDuration );
  //------------------------------------------------------------------------------------------------
  // Test daily recurrence that only lasts X days, and give start == end == last day of
  // recurrence. Previous versions of kcal had a bug and didn't return an occurrence
  start = start.addDays( recurrenceDuration - 1 );
  end = start;
  start.setTime( QTime( 0, 0, 0 ) );
  end.setTime( QTime( 23, 59, 59 ) );

  dateList = event->recurrence()->timesInInterval( start, end );
  QVERIFY( dateList.count() == 1 );

  //------------------------------------------------------------------------------------------------
}

//Test that interval start and end are inclusive
void TimesInIntervalTest::testSubDailyRecurrenceIntervalInclusive()
{
  const KDateTime start(QDate(2013, 03, 10), QTime(10, 0, 0), KDateTime::UTC);
  const KDateTime end(QDate(2013, 03, 10), QTime(11, 0, 0), KDateTime::UTC);

  KCalCore::Event::Ptr event(new KCalCore::Event());
  event->setUid("event");
  event->setDtStart(start);
  event->recurrence()->setHourly(1);
  event->recurrence()->setDuration(2);

  QList<KDateTime> expectedEventOccurrences;
  expectedEventOccurrences << start << start.addSecs(60*60);

  const DateTimeList timesInInterval = event->recurrence()->timesInInterval(start, end);
//   kDebug() << "timesInInterval " << timesInInterval;
  foreach (const KDateTime &dt, timesInInterval) {
//     kDebug() << dt;
    QCOMPARE(expectedEventOccurrences.removeAll(dt), 1);
  }
  QCOMPARE(expectedEventOccurrences.size(), 0);
}

//Test that the recurrence dtStart is used for calculation and not the interval start date
void TimesInIntervalTest::testSubDailyRecurrence2()
{
  const KDateTime start(QDate(2013, 03, 10), QTime(10, 2, 3), KDateTime::UTC);
  const KDateTime end(QDate(2013, 03, 10), QTime(13, 4, 5), KDateTime::UTC);

  KCalCore::Event::Ptr event(new KCalCore::Event());
  event->setUid("event");
  event->setDtStart(start);
  event->recurrence()->setHourly(1);
  event->recurrence()->setDuration(2);

  QList<KDateTime> expectedEventOccurrences;
  expectedEventOccurrences << start << start.addSecs(60*60);

  const DateTimeList timesInInterval = event->recurrence()->timesInInterval(start.addSecs(-20), end.addSecs(20));
//   kDebug() << "timesInInterval " << timesInInterval;
  foreach (const KDateTime &dt, timesInInterval) {
//     kDebug() << dt;
    QCOMPARE(expectedEventOccurrences.removeAll(dt), 1);
  }
  QCOMPARE(expectedEventOccurrences.size(), 0);
}

void TimesInIntervalTest::testSubDailyRecurrenceIntervalLimits()
{
  const KDateTime start(QDate(2013, 03, 10), QTime(10, 2, 3), KDateTime::UTC);
  const KDateTime end(QDate(2013, 03, 10), QTime(12, 2, 3), KDateTime::UTC);

  KCalCore::Event::Ptr event(new KCalCore::Event());
  event->setUid("event");
  event->setDtStart(start);
  event->recurrence()->setHourly(1);
  event->recurrence()->setDuration(3);

  QList<KDateTime> expectedEventOccurrences;
  expectedEventOccurrences << start.addSecs(60*60);

  const DateTimeList timesInInterval = event->recurrence()->timesInInterval(start.addSecs(1), end.addSecs(-1));
//   kDebug() << "timesInInterval " << timesInInterval;
  foreach (const KDateTime &dt, timesInInterval) {
//     kDebug() << dt;
    QCOMPARE(expectedEventOccurrences.removeAll(dt), 1);
  }
  QCOMPARE(expectedEventOccurrences.size(), 0);
}

//Test that the recurrence dtStart is used for calculation and not the interval start date
void TimesInIntervalTest::testDailyRecurrenceDtStart()
{
  const int days = 7;
  const KDateTime start(QDate(2013, 03, 10), QTime(0, 0, 0), KDateTime::ClockTime);
  const KDateTime intervalEnd = start.addDays(days);
  const KDateTime intervalStart = start.addDays(-days);

  KCalCore::Event::Ptr event(new KCalCore::Event());
  event->setUid("event");
  event->setDtStart(start);
  event->setDtEnd( start.addDays( 1 ) );
  event->setAllDay( true );
  event->setSummary( "Event2 Summary" );

  event->recurrence()->setDaily( 1 );

  QList<KDateTime> expectedEventOccurrences;
  for (int i = 0; i < days; ++i) {
    expectedEventOccurrences << start.addDays(i);
  }

  const DateTimeList timesInInterval = event->recurrence()->timesInInterval(intervalStart, intervalEnd);
  foreach (const KDateTime &dt, timesInInterval) {
    QCOMPARE(expectedEventOccurrences.removeAll(dt), 1);
  }

  QCOMPARE(expectedEventOccurrences.size(), 0);
}

//Test that the recurrence dtStart is used for calculation and not the interval start date
void TimesInIntervalTest::testWeeklyDayOfWeekRecurrenceDtStart()
{
  // Create an all-day event which occurs every weekday of every week,
  // starting from Friday the 11th of October.
  KCalCore::Event::Ptr event = KCalCore::Event::Ptr(new KCalCore::Event());
  event->startUpdates();
  event->setUid("event");
  event->setLocation(QStringLiteral("Test location"));
  event->setAllDay(true);
  event->setDescription(QStringLiteral("Test description"));
  event->setDtStart(KDateTime::fromString(QStringLiteral("2019-10-11T00:00:00+10:00")));
  event->setDtEnd(KDateTime::fromString(QStringLiteral("2019-10-12T00:00:00+10:00")));
  event->setSummary(QStringLiteral("Test event summary"));
  event->setCategories(QStringList() << QStringLiteral("Category One"));

  RecurrenceRule * const rule = new RecurrenceRule();
  rule->setRecurrenceType(RecurrenceRule::rWeekly);
  rule->setStartDt(event->dtStart());
  rule->setFrequency(1);
  rule->setByDays(QList<RecurrenceRule::WDayPos>() << RecurrenceRule::WDayPos(0, 1)   // monday
                                                   << RecurrenceRule::WDayPos(0, 2)   // tuesday
                                                   << RecurrenceRule::WDayPos(0, 3)   // wednesday
                                                   << RecurrenceRule::WDayPos(0, 4)   // thursday
                                                   << RecurrenceRule::WDayPos(0, 5)); // friday

  event->recurrence()->addRRule(rule);
  event->endUpdates();

  // Expand the events and within a wider interval, but ensure that the
  // expansion does not include e.g. Wednesday the 9th of October.
  const int days = 7;
  const DateTimeList timesInInterval = event->recurrence()->timesInInterval(
          event->dtStart().addDays(-days),
          event->dtEnd().addDays(days));

  QList<KDateTime> expectedEventOccurrences;
  for (int i = 0; i <= days; ++i) {
    // skip the saturday and sunday as those should not be expected
    if (i != 1 && i != 2) {
      expectedEventOccurrences << event->dtStart().addDays(i);
    }
  }

  QCOMPARE(expectedEventOccurrences.size(), timesInInterval.size());
  foreach (const KDateTime &dt, timesInInterval) {
    QCOMPARE(expectedEventOccurrences.removeAll(dt), 1);
  }

  QCOMPARE(expectedEventOccurrences.size(), 0);
}
