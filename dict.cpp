#include<iostream>
#include<string>
#include<any>
#include<unordered_map>
#include<tuple>
#include<cassert>
#include<algorithm>

// only for this file not to be copied to NSL ===========================
#include <complex>
#include <type_traits>
// ======================================================================
//

// TODO (@Copy to NSL)
// namespace NSL {

/*! Configuration storing a key(Field Name)-value(Field) pair
 *
 *  The keys are stored as std::strings while the values can be any type
 *  determined by Types, most likely you want to use NSL::Tensor to represent
 *  any field used in the MCMC implementation. 
 * */
template<typename ... Types>
class Configuration {
    public:

    //! Helper to enable operator[]
    /*!
     *  The return type deduction of `Configuration::operator[]` requires
     *  that either `Types` are all the same or there is only one Type specified.
     *  In these cases `Configuration::isHomogeneus::value` is true and
     *  is used to enable `operator[]`. Otherwise, `Configuration::isHomogeneus::value`
     *  is false and the `operator[]` can not be implemented!
     *
     *  `isHomogeneus::type` is aliasing the first template argument `Type`
     * */
    template<typename Type, typename ... OtherTypes>
    struct isHomogeneus {
        static constexpr bool value = (std::is_same<Type,OtherTypes>::value && ...) || sizeof...(Types) == 1;
        using type = Type;       
    };
    
 
    //! Configuration creation with key-value pair.
    /*! 
     * The Configuration is defined as a set of fields (NSL::Tensor) 
     *
     * */
    // TODO (@Copy to NSL): specializes to 
    //Configuration(std::pair<std::string,NSL::Tensor<Types>> ... keyVal) :
    Configuration(std::pair<std::string,Types> ... keyVal) :
        dict_{ {std::get<0>(keyVal),std::get<1>(keyVal)} ... }
    {}

    //! Access the field with a given key
    /*!
     * This function requires to specify the desired dtype which should 
     * match the original dtype of the field.
     * There is no (runtime) deduction of the return type.
     *
     *
     * */
    template<typename Type>
    Type field(std::string key){
        assert( dict_[key].type() == typeid(Type) );
        return std::any_cast<Type>(dict_[key]);
    }
    
    //! Acces the field with a given key (Homogeneus Configuration only)
    /*!
     * Provided as convenience function. If the configuration is considered
     * Homogeneus (all fields have the same type, see `Configuration::isHomogeneus`)
     * a non templated access method is available. 
     * */
    typename isHomogeneus<Types...>::type field(std::string key)
        requires ( isHomogeneus<Types...>::value )
    {
        return std::any_cast<typename isHomogeneus<Types...>::type>(dict_[key]);
    }

    //! Access the field names
    std::array<std::string,sizeof...(Types)> fieldNames(){
        std::array<std::string,sizeof...(Types)> names;
    
        std::transform(
            this->dict_.begin(),
            this->dict_.end(),
            names.begin(),
            [](const auto & x) {return x.first;}
        );
        
        return names;
    }

    private:
    std::unordered_map<std::string, std::any> dict_;

};



// TODO (@Copy to NSL)
// } // namespace NSL

// only for this file not to be copied to NSL ===========================
int main(){
    // Examples:
    // 1. Non-Homogeneus Configuration
    int spin = -1;
    double phase = 3.1415/2;
    std::complex<double> auxField = std::exp( std::complex<double>{0,1.23} ) ;
    
    Configuration<int,double,std::complex<double>> config( 
        {"spin",spin}, 
        {"U",phase}, 
        {"phi",auxField} 
    );

    // Compilation error for non-homogeneus configurations:
    // std::cout << "spin: " << config.field("spin") << std::endl;
    std::cout << "spin: " << config.field<int>("spin") << std::endl;
    std::cout << "U   : " << config.field<double>("U") << std::endl;
    std::cout << "phi : " << config.field<std::complex<double>>("phi") << std::endl;

    // Access all keys:
    std::cout << "[ ";
    for(auto fName: config.fieldNames()){
        std::cout << fName << " ";
    }
    std::cout << "]" << std::endl;

   Configuration<int,int,int> homConfig( 
        {"spin 1",spin}, 
        {"spin 2",spin}, 
        {"spin 3",spin} 
    );

   // for homogeneous configs it works :)
   std::cout << "spin 1 = " << homConfig.field("spin 1") << std::endl;
   std::cout << "spin 2 = " << homConfig.field("spin 2") << std::endl;
   std::cout << "spin 3 = " << homConfig.field("spin 3") << std::endl;


    return EXIT_SUCCESS;
}
// ======================================================================
