#include "Quiz.h"
#include "Question.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

std::vector<Question> createQuestions() {
    std::vector<Question> questions;

    Question q1;
    q1.text = "На юмористической картинке ОНА стала причиной спора архитекторов и орнитологов. Назовите ЕЁ.";
    q1.answer = "избушка на курьих ножках";
    q1.alternatives = { "изба на курьих ножках", "изба бабы яги", "избушка бабы яги" };
    q1.explanation = "Архитекторы и орнитологи не могли решить, чей это объект исследования.";
    q1.image = "izba.jpg";
    q1.category = "Искусство";
    q1.difficulty = 5;
    questions.push_back(q1);

    Question q2;
    q2.text = "В начале девятнадцатого века шотландский замок Дамбартон был хорошо укреплён. Дамбартон рассматривался как один из вариантов, но предпочли ЕГО. Назовите ЕГО тремя словами.";
    q2.answer = "остров святой елены";
    q2.alternatives = { "остров св.елены" };
    q2.explanation = "Замок рассматривался как кандидат на место заточения Наполеона.";
    q2.image = "ostrov_sv_elen.jpg";
    q2.category = "История";
    q2.difficulty = 7;
    questions.push_back(q2);

    Question q3;
    q3.text = "Зуб мезозойской родственницы мако был найден застрявшим между третьим и четвёртым ИКСАМИ. Назвать ИКС двумя словами на одну букву.";
    q3.answer = "позвонок птерозавра";
    q3.alternatives = { "позвонок птеранодона" };
    q3.explanation = "Зуб застрял между шейными позвонками летящего птеранодона.";
    q3.image = "Pteranodon_reconstruction.jpg";
    q3.category = "Палеонтология";
    q3.difficulty = 8;
    questions.push_back(q3);

    Question q4;
    q4.text = "Статья под названием 'Человек, который не любил мыть посуду' посвящена нобелевскому лауреату. Напишите его фамилию.";
    q4.answer = "Флеминг";
    q4.alternatives = { "александр флеминг", "alexander fleming" };
    q4.explanation = "Александр Флеминг открыл пенициллин благодаря тому, что вовремя не вымыл лабораторную посуду.";
    q4.image = "fleming.jpg";
    q4.category = "Наука";
    q4.difficulty = 5;
    questions.push_back(q4);

    Question q5;
    q5.text = "В Великобритании выпустили монету, посвящённую известному персонажу. На монете написано множество произведений. Предполагается, что обладатель монеты уподобится этому персонажу и воспользуется ЕЮ. Чем же?";
    q5.answer = "лупа";
    q5.alternatives = { "лупой", "увеличительное стекло" };
    q5.explanation = "Монета посвящена Шерлоку Холмсу, который часто пользовался лупой для расследований.";
    q5.image = "pence_holms.jpg";
    q5.category = "Литература";
    q5.difficulty = 6;
    questions.push_back(q5);

    unsigned seed = static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count());
    std::default_random_engine rng(seed);
    std::shuffle(questions.begin(), questions.end(), rng);

    return questions;
}

void printHeader(std::string title) {
    std::cout << "\n========================================\n";
    std::cout << title << "\n";
    std::cout << "========================================\n";
}

void printQuestion(Question q, int num, int total) {
    std::cout << "\n========== Вопрос " << num << " из " << total << " ==========\n\n";
    std::cout << q.text << "\n\n";
}

void printResult(bool correct) {
    if (correct) {
        std::cout << "\n========================================\n";
        std::cout << "ПРАВИЛЬНО! +1 балл\n";
        std::cout << "========================================\n\n";
    }
    else {
        std::cout << "\n========================================\n";
        std::cout << "НЕПРАВИЛЬНО!\n";
        std::cout << "========================================\n\n";
    }
}