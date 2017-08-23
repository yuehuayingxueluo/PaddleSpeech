#ifndef SCORER_H_
#define SCORER_H_

#include <string>
#include <memory>
#include <vector>
#include "lm/enumerate_vocab.hh"
#include "lm/word_index.hh"
#include "lm/virtual_interface.hh"
#include "util/string_piece.hh"

const double OOV_SCOER = -1000.0;
const std::string START_TOKEN = "<s>";
const std::string UNK_TOKEN = "<unk>";
const std::string END_TOKEN = "</s>";

 // Implement a callback to retrive string vocabulary.
class RetriveStrEnumerateVocab : public lm::EnumerateVocab {
public:
    RetriveStrEnumerateVocab() {}

    void Add(lm::WordIndex index, const StringPiece& str) {
        vocabulary.push_back(std::string(str.data(), str.length()));
    }

    std::vector<std::string> vocabulary;
};

// External scorer to query languange score for n-gram or sentence.
// Example:
//     Scorer scorer(alpha, beta, "path_of_language_model");
//     scorer.get_log_cond_prob({ "WORD1", "WORD2", "WORD3" });
//     scorer.get_sent_log_prob({ "WORD1", "WORD2", "WORD3" });
class Scorer{
public:
    Scorer(double alpha, double beta, const std::string& lm_path);
    ~Scorer();
    double get_log_cond_prob(const std::vector<std::string>& words);
    double get_sent_log_prob(const std::vector<std::string>& words);
    size_t get_max_order() { return _max_order; }
    bool is_character_based() { return _is_character_based; }
    std::vector<std::string> get_vocab() { return _vocabulary; }

    // expose to decoder
    double alpha;
    double beta;

protected:
    void load_LM(const char* filename);
    double get_log_prob(const std::vector<std::string>& words);

private:
    void* _language_model;
    bool _is_character_based;
    size_t _max_order;
    std::vector<std::string> _vocabulary;
};

#endif // SCORER_H_