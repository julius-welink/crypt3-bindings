declare module 'crypt3-bindings' {
    function encrypt(passwd: string, salt?: string): string;
    function verify(passwd: string, hash: string): boolean;

    export {
        encrypt,
        verify,
    }
}