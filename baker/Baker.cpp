#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
#include "../includes/box.h"
#include "../includes/PRINT.h"
using namespace std;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

void Baker::bake_and_box(ORDER &anOrder) {
	int donuts = 0;
	Box b;
	DONUT d;
	while (donuts != anOrder.number_donuts) {
		b.addDonut(d);
		donuts += 1;
		if (b.size() == DOZEN) {
			anOrder.boxes.push_back(b);
			b.clear();
		}
	}
	if (b.size() > 0) {
		anOrder.boxes.push_back(b);
	}
}

void Baker::beBaker() {
	while (true) {
		unique_lock<mutex> lck(mutex_order_inQ);
		while (!b_WaiterIsFinished && !(order_in_Q.size() > 0)) {
			cv_order_inQ.wait(lck);
		}
		if (order_in_Q.size() > 0) {
			bake_and_box(order_in_Q.front());
			order_out_Vector.push_back(order_in_Q.front());
			order_in_Q.pop();
		}
		if (b_WaiterIsFinished && order_in_Q.size() == 0) {
			break;
		}
	}
}
