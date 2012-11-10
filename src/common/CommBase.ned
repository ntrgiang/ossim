package so.common;

simple CommBase
{
    parameters:
        @class(CommBase);

    gates:
        // -- Gates to the Dispatcher module
        input dpIn @labels(DpControlInfo/up);
        output dpOut @labels(DpControlInfo/down);
}
