#pragma once

/*----------------
	GlobalQueue
-----------------*/

namespace FrokEngine
{
	class GlobalQueue
	{
	public:
		GlobalQueue();
		~GlobalQueue();

		void					Push(JobQueueRef jobQueue);
		JobQueueRef				Pop();

	private:
		LockQueue<JobQueueRef> _jobQueues;
	};

}
