#include <iostream>
#include <memory>
#include "MysqlConn.h"
#include "ConnectionPool.h"
using namespace std;

void op1(int begin, int end)
{
	for (int i = begin; i < end; ++i)
	{
		MysqlConn conn;
		conn.connect("root", "010223", "student_manager", "127.0.0.1");
		char sql[1024] = { 0 };
		sprintf(sql, "insert into students values(%d, 'xiebaowanger', 'beishida')", i);
		conn.update(sql);
	}
}

void op2(ConnectionPool* pool, int begin, int end)
{
	for (int i = begin; i < end; ++i)
	{
		shared_ptr<MysqlConn> conn = pool->getConnection();
		char sql[1024] = { 0 };
		sprintf(sql, "insert into students values(%d, 'xiebaowanger', 'beishida')", i);
		conn->update(sql);
	}
}

void test1()
{
#if 0
	//non connectionpool, single thread, time6289777800ns, 6289ms,
	steady_clock::time_point begin = steady_clock::now();
	op1(0, 5000);
	steady_clock::time_point end = steady_clock::now();
	auto length = end - begin;
	cout << "non connectionpool, single thread, time" << length.count() <<
		"ns, " << length.count() / 1000000 << "ms, " << endl;
#else 
	//non connectionpool, multi thread, time4972550300ns, 4972ms,
	ConnectionPool* pool = ConnectionPool::getConnectPool();
	steady_clock::time_point begin = steady_clock::now();
	op2(pool, 0, 5000);
	steady_clock::time_point end = steady_clock::now();
	auto length = end - begin;
	cout << "non connectionpool, single thread, time" << length.count() <<
		"ns, " << length.count() / 1000000 << "ms, " << endl;
#endif
}
void test2()
{
#if 1
	//non connectionpool, multi thread, time5103040800ns, 5103ms,
	MysqlConn conn;
	conn.connect("root", "010223", "student_manager", "127.0.0.1");
	steady_clock::time_point begin = steady_clock::now();
	thread t1(op1, 0, 1000);
	thread t2(op1, 1000, 2000);
	thread t3(op1, 2000, 3000);
	thread t4(op1, 3000, 4000);
	thread t5(op1, 4000, 5000);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	steady_clock::time_point end = steady_clock::now();
	auto length = end - begin;
	cout << "non connectionpool, multi thread, time" << length.count() <<
		"ns, " << length.count() / 1000000 << "ms, " << endl;
#else
	ConnectionPool* pool = ConnectionPool::getConnectPool();
	steady_clock::time_point begin = steady_clock::now();
	thread t1(op2, pool, 0, 1000);
	thread t2(op2, pool, 1000, 2000);
	thread t3(op2, pool, 2000, 3000);
	thread t4(op2, pool, 3000, 4000);
	thread t5(op2, pool, 4000, 5000);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	steady_clock::time_point end = steady_clock::now();
	auto length = end - begin;
	cout << "non connectionpool, multi thread, time" << length.count() <<
		"ns, " << length.count() / 1000000 << "ms, " << endl;
#endif
}

int query()
{
	MysqlConn conn;
	conn.connect("root", "010223", "student_manager", "127.0.0.1");
	string sql = "insert into students values(11007, 'dd', 'a')";
	bool flag = conn.update(sql);
	cout << "flag value: " << flag << endl;

	sql = "select * from students";
	conn.query(sql);
	while (conn.next())
	{
		cout << conn.value(0) << ", "
			<< conn.value(1) << ", "
			<< conn.value(2) << endl;
	}
	return 0;
}
int main()
{
	test2();
	return 0;
}