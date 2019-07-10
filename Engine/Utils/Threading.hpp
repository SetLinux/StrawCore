#include <tbb/tbb.h>
#include "../entt/entt.hpp"
namespace Straw{
namespace Threading {
static tbb::task_group grp;
template<typename T,typename S>
void MakeViewParrarel(T& view,std::function<void(S)> func,int numofworkers) {
float StepSize = (float) view.size() / numofworkers;

for(int i =0; i < numofworkers; i++){
    grp.run([&](){
    int counter = 0;
        for(auto& it : view){
        counter++;
        if(counter > i * StepSize; counter <= (i+1) * StepSize)
            func(it);
    }

    });

}
grp.wait();
}

} // namespace Threading
}
