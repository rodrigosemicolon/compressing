#include<iostream>
#include<map>
#include<string>
#include<regex>
#include<list>
#include<fstream>

#define DELIM ","

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::list;
using std::iterator;



class LZ78_Decoder
{
    public:
    std::map<int, string> dict;
    int dict_size=0;

    void decompress_file(string filename, string decompressed_filename){
        using std::fstream;
        using std::ios;

        fstream fread;
        fread.open(filename, ios::in);
        if(!fread){
            cout << "File not found." << endl;
            fread.close();
            return;
        }
        else{
            string compressed_file;
            while(true){
                if(fread.eof()){
                    break;
                }
                fread >> compressed_file;
            }
            
            fread.close();
            fstream fwrite;
            fwrite.open(decompressed_filename,ios::out);
            if(!fwrite){
                cout << "Error creating file."<<endl;
                fwrite.close();
                return;
            }
            else{
                
                fwrite <<  decompress(read_codewords(compressed_file));
                fwrite.close();
            }
        }
        
    }
    string decompress(list<string> codewords){
        std::list<string>::iterator it;
        string original;
        for(it=codewords.begin(); it!=codewords.end(); ++it){
            int dict_pos;
            char unknown;
            int delim_pos = (*it).find(DELIM, 0);
            dict_pos = std::stoi( (*it).substr(0,delim_pos));
            unknown = (*it)[delim_pos+1];
            
            if(dict_pos == 0){
                dict_size++;
                dict[dict_size] = unknown;      
                original+=unknown;    
            }
            else{
                dict_size++;
                string new_entry = dict[dict_pos] + unknown;
                dict[dict_size] = new_entry;
                original+=new_entry;
            }
            
        }
        return original;
    }
    std::list<string> read_codewords(string encoded){
        using std::list;
        using std::regex;
        using std::regex_search;
        list<string> decoded;
        regex r("([0-9]+,[a-z]+)");
        std::smatch m;
        
        while(regex_search(encoded,m, r)){
        
            
            
            decoded.push_back(m.str());  
            
            
            encoded = m.suffix().str();
          
        }
           
        
        return decoded;
    }

};
class LZ78_Encoder
{
    public:
    std::map<string, int> dict;
    int dict_size=0;

    void compress_file(string filename, string compressed_filename){
        using std::fstream;
        using std::ios;
        fstream fread;
        fread.open(filename, ios::in);
        if(!fread){
            cout << "File not found." << endl;
            fread.close();
            return;
        }
        else{
            string original_file;
            while(true){
                if(fread.eof()){
                    break;
                }
                fread >> original_file;
            }
            
            fread.close();
            fstream fwrite;
            fwrite.open(compressed_filename,ios::out);
            if(!fwrite){
                cout << "Error creating file."<<endl;
                fwrite.close();
                return;
            }
            else{
                
                fwrite <<  compress(original_file);
                fwrite.close();
            }
        }
        
    }

    string compress(string &text){
        string compressed_text = "";
        string current_prefix="";
        int size = text.length();
        int i=0;
        bool matched;
        for(char const &c: text){
            i++;
            current_prefix+=c;
            matched=false;
            if (dict.find(current_prefix)==dict.end()){
                dict_size++;
                matched=true;
                dict[current_prefix]=dict_size;
                current_prefix.pop_back();
                
               
                
                compressed_text+=mini_encode(get_largest_match(current_prefix),c);
                
                current_prefix="";
            }
            if(matched==false && i==size){
                current_prefix.pop_back();
                compressed_text+=mini_encode(get_largest_match(current_prefix),c);
            }
        }
        
    return compressed_text;

    }


    string mini_encode(int dict_pos, char unknown){
        return '(' + std::to_string(dict_pos) + ',' + unknown + ')';
    }


    int get_largest_match(string a){
        if(a.length()==0){
            return 0;
        }
        else{
            return dict.find(a)->second;
        }
    }

    


    void print_dict()
    {   
        std::map<string,int>::iterator itr;
        std::cout << "Dictionary:\n";
        for(itr = dict.begin(); itr!=dict.end(); ++itr){
            std::cout << itr->first << ": " << itr->second << "\n";
        }
    }
};



int main(int argc, char *argv[])
{
    
    if(argc==1 ||strcmp(argv[1],"-h")==0 ){
        string message = "\tPossible arguments:\n\t\t-comp <file_to_compress> <compressed_destination>\n\t\t-decomp <file_to_decompress> <decompressed_destination>\n";
        cout << message << endl;
    }
    else if (argc==4){
        
        string original = string(argv[2]);
        string destination = string(argv[3]);
        if(strcmp(argv[1],"-comp")==0){
            LZ78_Encoder encoder;
            encoder.compress_file(original, destination);
        }
        else if(strcmp(argv[1],"-decomp")==0){
            LZ78_Decoder decoder;
            decoder.decompress_file(original, destination);
        }
        else{
            cout << "Invalid argument." << endl;
        }
    }
    else{
        cout << "Invalid number of arguments." << endl;
    }

    
    return 0;


}