function result = sinxDividex(x)
    if (abs(x) < 0.000000001)
        result = 1;
    else
        result = sin(x) / x;
    end
end