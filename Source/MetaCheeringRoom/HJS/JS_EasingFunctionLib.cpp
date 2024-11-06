// Fill out your copyright notice in the Description page of Project Settings.


#include "HJS/JS_EasingFunctionLib.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"

float UJS_EasingFunctionLib::ApplyElasticEaseOut(float Start, float End, float Alpha)
{
    return UKismetMathLibrary::Lerp(Start, End, ElasticEaseOut(Alpha));
}

float UJS_EasingFunctionLib::ApplyBounceEaseIn(float Start, float End, float Alpha)
{
    return UKismetMathLibrary::Lerp(Start, End, BounceEaseIn(Alpha));
}

float UJS_EasingFunctionLib::ElasticEaseOut(float Alpha)
{
    const float c4 = (2 * PI) / 3;
    return (Alpha == 0) ? 0 : (Alpha == 1) ? 1
        : pow(2, -10 * Alpha) * sin((Alpha * 10 - 0.75) * c4) + 1;
}

float UJS_EasingFunctionLib::BounceEaseIn(float Alpha)
{
    return 1 - BounceEaseOut(1 - Alpha);
}

float UJS_EasingFunctionLib::BounceEaseOut(float Alpha)
{
    if (Alpha < 1 / 2.75f)
    {
        return 7.5625f * Alpha * Alpha;
    }
    else if (Alpha < 2 / 2.75f)
    {
        Alpha -= 1.5f / 2.75f;
        return 7.5625f * Alpha * Alpha + 0.75f;
    }
    else if (Alpha < 2.5 / 2.75)
    {
        Alpha -= 2.25f / 2.75f;
        return 7.5625f * Alpha * Alpha + 0.9375f;
    }
    else
    {
        Alpha -= 2.625f / 2.75f;
        return 7.5625f * Alpha * Alpha + 0.984375f;
    }
}

float UJS_EasingFunctionLib::FadeInEase(float Alpha)
{
    return FMath::Clamp(Alpha * Alpha, 0.0f, 1.0f);
}

float UJS_EasingFunctionLib::FadeOutEase(float Alpha)
{
    return FMath::Clamp(1.0f - (Alpha * Alpha), 0.0f, 1.0f);
}